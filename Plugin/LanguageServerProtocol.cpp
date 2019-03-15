#include "LanguageServerProtocol.h"
#include "file_logger.h"
#include "processreaderthread.h"
#include "LSP/clJSONRPC.h"
#include "LSP/GotoDefinitionRequest.h"
#include "LSP/GotoDeclarationRequest.h"
#include "LSP/DidOpenTextDocumentRequest.h"
#include "LSP/DidCloseTextDocumentRequest.h"
#include "LSP/DidSaveTextDocumentRequest.h"
#include "LSP/DidChangeTextDocumentRequest.h"
#include "LSP/CompletionRequest.h"
#include "LSP/InitializeRequest.h"
#include "LSP/ResponseMessage.h"
#include "LSP/ResponseError.h"
#include "event_notifier.h"
#include "codelite_events.h"
#include "fileextmanager.h"
#include "ieditor.h"
#include "globals.h"
#include "imanager.h"
#include <wx/stc/stc.h>
#include "LSP/LSPEvent.h"
#include "clWorkspaceManager.h"
#include <wx/stc/stc.h>
#include <wx/filesys.h>
#include <iomanip>
#include <sstream>
#include "LSPNetworkSocket.h"

#define PORT 12989

LanguageServerProtocol::LanguageServerProtocol(const wxString& name, wxEvtHandler* owner)
    : m_name(name)
    , m_owner(owner)
{
    EventNotifier::Get()->Bind(wxEVT_FILE_SAVED, &LanguageServerProtocol::OnFileSaved, this);
    EventNotifier::Get()->Bind(wxEVT_FILE_CLOSED, &LanguageServerProtocol::OnFileClosed, this);
    EventNotifier::Get()->Bind(wxEVT_FILE_LOADED, &LanguageServerProtocol::OnFileLoaded, this);
    EventNotifier::Get()->Bind(wxEVT_WORKSPACE_CLOSED, &LanguageServerProtocol::OnWorkspaceClosed, this);
    EventNotifier::Get()->Bind(wxEVT_WORKSPACE_LOADED, &LanguageServerProtocol::OnWorkspaceOpen, this);

    // Use sockets here
    m_network.reset(new LSPNetworkSocket());
    m_network->Bind(wxEVT_LSP_NET_DATA_READY, &LanguageServerProtocol::OnNetDataReady, this);
    m_network->Bind(wxEVT_LSP_NET_ERROR, &LanguageServerProtocol::OnNetError, this);
    m_network->Bind(wxEVT_LSP_NET_CONNECTED, &LanguageServerProtocol::OnNetConnected, this);
}

LanguageServerProtocol::~LanguageServerProtocol()
{
    EventNotifier::Get()->Unbind(wxEVT_FILE_SAVED, &LanguageServerProtocol::OnFileSaved, this);
    EventNotifier::Get()->Unbind(wxEVT_FILE_CLOSED, &LanguageServerProtocol::OnFileClosed, this);
    EventNotifier::Get()->Unbind(wxEVT_FILE_LOADED, &LanguageServerProtocol::OnFileLoaded, this);
    EventNotifier::Get()->Unbind(wxEVT_WORKSPACE_CLOSED, &LanguageServerProtocol::OnWorkspaceClosed, this);
    EventNotifier::Get()->Unbind(wxEVT_WORKSPACE_LOADED, &LanguageServerProtocol::OnWorkspaceOpen, this);
    DoClear();
}

wxString LanguageServerProtocol::GetLanguageId(const wxString& fn)
{
    FileExtManager::FileType type = FileExtManager::GetType(fn, FileExtManager::TypeText);
    switch(type) {
    case FileExtManager::TypeSourceC:
        return "c";
    case FileExtManager::TypeSourceCpp:
    case FileExtManager::TypeHeader:
        return "cpp";
    case FileExtManager::TypeJS:
        return "javascript";
    case FileExtManager::TypeDockerfile:
        return "dockerfile";
    case FileExtManager::TypeJava:
        return "java";
    case FileExtManager::TypePython:
        return "python";
    case FileExtManager::TypeMakefile:
        return "makefile";
    case FileExtManager::TypePhp:
        return "php";
    case FileExtManager::TypeSQL:
        return "sql";
    default:
        return "";
    }
}

const std::set<wxString>& LanguageServerProtocol::GetSupportedLanguages()
{
    static std::set<wxString> S;
    if(S.empty()) {
        S.insert("bat");
        S.insert("bibtex");
        S.insert("clojure");
        S.insert("coffeescript");
        S.insert("c");
        S.insert("cpp");
        S.insert("csharp");
        S.insert("css");
        S.insert("diff");
        S.insert("dart");
        S.insert("dockerfile");
        S.insert("fsharp");
        S.insert("git-commit");
        S.insert("git-rebase");
        S.insert("go");
        S.insert("groovy");
        S.insert("handlebars");
        S.insert("html");
        S.insert("ini");
        S.insert("java");
        S.insert("javascript");
        S.insert("json");
        S.insert("latex");
        S.insert("less");
        S.insert("lua");
        S.insert("makefile");
        S.insert("markdown");
        S.insert("objective-c");
        S.insert("objective-cpp");
        S.insert("perl and perl6");
        S.insert("php");
        S.insert("powershell");
        S.insert("jade");
        S.insert("python");
        S.insert("r");
        S.insert("razor");
        S.insert("ruby");
        S.insert("rust");
        S.insert("scss");
        S.insert("sass");
        S.insert("scala");
        S.insert("shaderlab");
        S.insert("shellscript");
        S.insert("sql");
        S.insert("swift");
        S.insert("typescript");
        S.insert("tex");
        S.insert("vb");
        S.insert("xml");
        S.insert("xsl");
        S.insert("yaml");
    }
    return S;
}

void LanguageServerProtocol::QueueMessage(LSP::RequestMessage::Ptr_t request)
{
    if(!IsInitialized()) { return; }
    m_Queue.Push(request);
    ProcessQueue();
}

void LanguageServerProtocol::DoStart()
{
    DoClear();

    if(m_command.IsEmpty()) { return; }
    wxString command;
    command << m_command.Item(0);
    ::WrapWithQuotes(command);

    for(size_t i = 1; i < m_command.size(); ++i) {
        command << " " << m_command.Item(i);
    }

    clDEBUG() << GetLogPrefix() << "Starting...";
    clDEBUG() << GetLogPrefix() << "Command:" << command;
    clDEBUG() << GetLogPrefix() << "Root folder:" << m_rootFolder;
    for(const wxString& lang : m_languages) {
        clDEBUG() << GetLogPrefix() << "Language:" << lang;
    }
    LSPNetwork::StartupInfo info = { m_helperCommand, command };
    m_network->Open(info);
}

void LanguageServerProtocol::Start(const wxString& helperCommand, const wxArrayString& argv, const wxString& rootFolder,
                                   const wxArrayString& languages)
{
    if(IsRunning()) { return; }
    DoClear();
    m_languages.clear();
    std::for_each(languages.begin(), languages.end(), [&](const wxString& lang) { m_languages.insert(lang); });
    m_command = argv;
    m_rootFolder = rootFolder;
    m_helperCommand = helperCommand;
    DoStart();
}

void LanguageServerProtocol::Start()
{
    if(IsRunning()) { return; }
    DoStart();
}

void LanguageServerProtocol::DoClear()
{
    m_filesSent.clear();
    m_outputBuffer.clear();
    m_state = kUnInitialized;
    m_initializeRequestID = wxNOT_FOUND;
    m_Queue.Clear();

    // Destory the current connection
    m_network->Close();
}

bool LanguageServerProtocol::IsRunning() const { return m_network->IsConnected(); }

bool LanguageServerProtocol::CanHandle(const wxFileName& filename) const
{
    wxString lang = GetLanguageId(filename);
    return m_languages.count(lang) != 0;
}

bool LanguageServerProtocol::ShouldHandleFile(const wxFileName& fn) const
{
    wxString lang = GetLanguageId(fn);
    return (m_languages.count(lang) != 0);
}

bool LanguageServerProtocol::ShouldHandleFile(IEditor* editor) const
{
    return editor && ShouldHandleFile(editor->GetFileName());
}

//===--------------------------------------------------
// Protocol implementation
//===--------------------------------------------------

void LanguageServerProtocol::FindDefinition(IEditor* editor)
{
    CHECK_PTR_RET(editor);
    CHECK_COND_RET(ShouldHandleFile(editor));

    // If the editor is modified, we need to tell the LSP to reparse the source file
    const wxFileName& filename = editor->GetFileName();
    if(m_filesSent.count(filename.GetFullPath()) && editor->IsModified()) {
        // we already sent this file over, ask for change parse
        SendChangeRequest(filename, editor->GetTextRange(0, editor->GetLength()));
    } else if(m_filesSent.count(filename.GetFullPath()) == 0) {
        SendOpenRequest(filename, editor->GetTextRange(0, editor->GetLength()), GetLanguageId(filename));
    }

    LSP::GotoDefinitionRequest::Ptr_t req = LSP::RequestMessage::MakeRequest(new LSP::GotoDefinitionRequest(
        editor->GetFileName(), editor->GetCurrentLine(), editor->GetCtrl()->GetColumn(editor->GetCurrentPosition())));
    QueueMessage(req);
}

void LanguageServerProtocol::SendOpenRequest(const wxFileName& filename, const wxString& fileContent,
                                             const wxString& languageId)
{
    LSP::DidOpenTextDocumentRequest::Ptr_t req =
        LSP::RequestMessage::MakeRequest(new LSP::DidOpenTextDocumentRequest(filename, fileContent, languageId));
    req->SetStatusMessage(wxString() << "[LSP] parsing file: " << filename.GetFullName());
    QueueMessage(req);
}

void LanguageServerProtocol::SendCloseRequest(const wxFileName& filename)
{
    if(m_filesSent.count(filename.GetFullPath()) == 0) {
        clDEBUG() << GetLogPrefix() << "LanguageServerProtocol::FileClosed(): file" << filename << "is not opened";
        return;
    }

    LSP::DidCloseTextDocumentRequest::Ptr_t req =
        LSP::RequestMessage::MakeRequest(new LSP::DidCloseTextDocumentRequest(filename));
    QueueMessage(req);
    m_filesSent.erase(filename.GetFullPath());
}

void LanguageServerProtocol::SendChangeRequest(const wxFileName& filename, const wxString& fileContent)
{
    LSP::DidChangeTextDocumentRequest::Ptr_t req =
        LSP::RequestMessage::MakeRequest(new LSP::DidChangeTextDocumentRequest(filename, fileContent));
    req->SetStatusMessage(wxString() << "[LSP] re-parsing file: " << filename.GetFullName());
    QueueMessage(req);
}

void LanguageServerProtocol::SendSaveRequest(const wxFileName& filename, const wxString& fileContent)
{
    // LSP::DidSaveTextDocumentRequest req(filename, fileContent);
    // req.Send(this);
    // clGetManager()->SetStatusMessage(wxString() << "[LSP] re-parsing file: " << filename.GetFullName(), 1);

    // For now: report a change event
    SendChangeRequest(filename, fileContent);
}

void LanguageServerProtocol::SendCodeCompleteRequest(const wxFileName& filename, size_t line, size_t column)
{
    if(ShouldHandleFile(filename)) {
        LSP::CompletionRequest::Ptr_t req = LSP::RequestMessage::MakeRequest(
            new LSP::CompletionRequest(LSP::TextDocumentIdentifier(filename), LSP::Position(line, column)));
        QueueMessage(req);
    }
}

//===------------------------------------------------------------------
// Event handlers
//===------------------------------------------------------------------
void LanguageServerProtocol::OnFileLoaded(clCommandEvent& event)
{
    event.Skip();
    IEditor* editor = clGetManager()->GetActiveEditor();
    OpenEditor(editor);
}

void LanguageServerProtocol::OnFileClosed(clCommandEvent& event)
{
    event.Skip();
    SendCloseRequest(event.GetFileName());
}

void LanguageServerProtocol::OnFileSaved(clCommandEvent& event)
{
    event.Skip();
    // For now, it does the same as 'OnFileLoaded'
    IEditor* editor = clGetManager()->GetActiveEditor();
    if(editor && ShouldHandleFile(editor)) { SendSaveRequest(editor->GetFileName(), editor->GetCtrl()->GetText()); }
}

wxString LanguageServerProtocol::GetLogPrefix() const { return wxString() << "LSP [" << GetName() << "]:"; }

void LanguageServerProtocol::OpenEditor(IEditor* editor)
{
    if(!IsInitialized()) { return; }
    if(editor && ShouldHandleFile(editor)) {
        if(m_filesSent.count(editor->GetFileName().GetFullPath())) {
            clDEBUG() << "OpenEditor->SendChangeRequest called for:" << editor->GetFileName().GetFullName();
            SendChangeRequest(editor->GetFileName(), editor->GetCtrl()->GetText());
        } else {
            clDEBUG() << "OpenEditor->SendOpenRequest called for:" << editor->GetFileName().GetFullName();
            SendOpenRequest(editor->GetFileName(), editor->GetCtrl()->GetText(), GetLanguageId(editor->GetFileName()));
        }
    }
}

void LanguageServerProtocol::CodeComplete(IEditor* editor)
{
    // sanity
    CHECK_PTR_RET(editor);
    CHECK_COND_RET(ShouldHandleFile(editor));

    // If the editor is modified, we need to tell the LSP to reparse the source file
    const wxFileName& filename = editor->GetFileName();
    if(m_filesSent.count(filename.GetFullPath()) && editor->IsModified()) {
        // we already sent this file over, ask for change parse
        SendChangeRequest(filename, editor->GetTextRange(0, editor->GetLength()));
    } else if(m_filesSent.count(filename.GetFullPath()) == 0) {
        SendOpenRequest(filename, editor->GetTextRange(0, editor->GetLength()), GetLanguageId(filename));
    }
    // Now request the for code completion
    SendCodeCompleteRequest(editor->GetFileName(), editor->GetCurrentLine(),
                            editor->GetCtrl()->GetColumn(editor->GetCurrentPosition()));
}

void LanguageServerProtocol::ProcessQueue()
{
    if(m_Queue.IsEmpty()) { return; }
    if(m_Queue.IsWaitingReponse()) {
        clDEBUG() << "LSP is busy, will not send message";
        return;
    }
    LSP::RequestMessage::Ptr_t req = m_Queue.Get();
    if(!IsRunning()) {
        clDEBUG() << GetLogPrefix() << "is down.";
        return;
    }

    // Write the message length as string of 10 bytes
    m_network->Send(req->ToString());
    m_Queue.SetWaitingReponse(true);
    m_Queue.Pop();
    if(!req->GetStatusMessage().IsEmpty()) { clGetManager()->SetStatusMessage(req->GetStatusMessage(), 1); }
}

void LanguageServerProtocol::CloseEditor(IEditor* editor)
{
    if(!IsInitialized()) { return; }
    if(editor && ShouldHandleFile(editor)) { SendCloseRequest(editor->GetFileName()); }
}

void LanguageServerProtocol::FindDeclaration(IEditor* editor)
{
    CHECK_PTR_RET(editor);
    CHECK_COND_RET(ShouldHandleFile(editor));

    // If the editor is modified, we need to tell the LSP to reparse the source file
    const wxFileName& filename = editor->GetFileName();
    if(m_filesSent.count(filename.GetFullPath()) && editor->IsModified()) {
        // we already sent this file over, ask for change parse
        SendChangeRequest(filename, editor->GetTextRange(0, editor->GetLength()));
    } else if(m_filesSent.count(filename.GetFullPath()) == 0) {
        SendOpenRequest(filename, editor->GetTextRange(0, editor->GetLength()), GetLanguageId(filename));
    }

    LSP::GotoDeclarationRequest::Ptr_t req = LSP::RequestMessage::MakeRequest(new LSP::GotoDeclarationRequest(
        editor->GetFileName(), editor->GetCurrentLine(), editor->GetCtrl()->GetColumn(editor->GetCurrentPosition())));
    QueueMessage(req);
}

void LanguageServerProtocol::OnNetConnected(clCommandEvent& event)
{
    // The process started successfully
    // Send the 'initialize' request
    LSP::InitializeRequest::Ptr_t req = LSP::RequestMessage::MakeRequest(new LSP::InitializeRequest());
    req->As<LSP::InitializeRequest>()->SetRootUri(m_rootFolder);

    // Temporarly set the state to "kInitialized" so we can send out the "initialize" request
    m_state = kInitialized;
    QueueMessage(req);
    m_state = kUnInitialized;
    m_initializeRequestID = req->GetId();
}

void LanguageServerProtocol::OnNetError(clCommandEvent& event)
{
    clDEBUG() << GetLogPrefix() << "Socket error." << event.GetString();
    DoClear();
}

void LanguageServerProtocol::OnNetDataReady(clCommandEvent& event)
{
    clDEBUG() << GetLogPrefix() << event.GetString();
    wxString buffer = std::move(event.GetString());
    m_outputBuffer << buffer;
    m_Queue.SetWaitingReponse(false);

    while(true) {
        // Did we get a complete message?
        LSP::ResponseMessage res(m_outputBuffer);
        if(res.IsOk()) {
            clDEBUG() << GetLogPrefix() << "received a complete message";

            if(IsInitialized()) {
                LSP::RequestMessage::Ptr_t msg_ptr = m_Queue.TakePendingReplyMessage(res.GetId());
                // Is this an error message?
                if(res.Has("error")) {
                    LSP::ResponseError errMsg(res.GetMessageString());
                    switch(errMsg.GetErrorCode()) {
                    case LSP::ResponseError::kErrorCodeInternalError:
                    case LSP::ResponseError::kErrorCodeInvalidRequest: {
                        // Restart this server
                        LSPEvent restartEvent(wxEVT_LSP_RESTART_NEEDED);
                        restartEvent.SetServerName(GetName());
                        AddPendingEvent(restartEvent);
                        break;
                    }
                    case LSP::ResponseError::kErrorCodeInvalidParams: {
                        // Recreate this AST (in other words: reparse), by default we reparse the current editor
                        LSPEvent reparseEvent(wxEVT_LSP_REPARSE_NEEDED);
                        reparseEvent.SetServerName(GetName());
                        AddPendingEvent(reparseEvent);
                        break;
                    }
                    default:
                        break;
                    }
                } else {
                    if(msg_ptr) {
                        // let the originating request to handle it
                        msg_ptr->OnResponse(res, m_owner);

                        // If we got more in the buffer, try to process another message
                        if(!m_outputBuffer.IsEmpty()) { continue; }

                    } else if(res.IsPushDiagnostics()) {
                        // Get the URI
                        JSONItem params = res.Get("params");
                        JSONItem uri = params.namedObject("uri");
                        wxFileName fn(wxFileSystem::URLToFileName(uri.toString()));
                        fn.Normalize();
                        clGetManager()->SetStatusMessage(
                            wxString() << "[LSP] parsing of file: " << fn.GetFullName() << " is completed", 1);
                    }
                }
            } else {
                // we only accept initialization responses here
                if(res.GetId() == m_initializeRequestID) {
                    clDEBUG() << GetLogPrefix() << "initialization completed";
                    m_initializeRequestID = wxNOT_FOUND;
                    m_state = kInitialized;

                    // Notify about this
                    LSPEvent initEvent(wxEVT_LSP_INITIALIZED);
                    initEvent.SetServerName(GetName());
                    AddPendingEvent(initEvent);
                }
            }
        }
        break;
    }
    ProcessQueue();
}

void LanguageServerProtocol::Stop()
{
    clDEBUG() << GetLogPrefix() << "Going down";
    m_network->Close();
}

void LanguageServerProtocol::OnWorkspaceClosed(wxCommandEvent& event)
{
    event.Skip();
    m_rootFolder.Clear();
    Stop();
    Start();
}

void LanguageServerProtocol::OnWorkspaceOpen(wxCommandEvent& event)
{
    event.Skip();
    m_rootFolder = wxFileName(event.GetString()).GetPath();
    Stop();
    Start();
}

//===------------------------------------------------------------------
// LSPRequestMessageQueue
//===------------------------------------------------------------------

void LSPRequestMessageQueue::Push(LSP::RequestMessage::Ptr_t message)
{
    m_Queue.push(message);
    if(message->IsNeedsReply()) { m_pendingReplyMessages.insert({ message->GetId(), message }); }
}

void LSPRequestMessageQueue::Pop()
{
    if(!m_Queue.empty()) { m_Queue.pop(); }
    SetWaitingReponse(false);
}

LSP::RequestMessage::Ptr_t LSPRequestMessageQueue::Get()
{
    if(m_Queue.empty()) { return LSP::RequestMessage::Ptr_t(nullptr); }
    return m_Queue.front();
}

void LSPRequestMessageQueue::Clear()
{
    while(!m_Queue.empty()) {
        m_Queue.pop();
    }
    SetWaitingReponse(false);
    m_pendingReplyMessages.clear();
}

LSP::RequestMessage::Ptr_t LSPRequestMessageQueue::TakePendingReplyMessage(int msgid)
{
    if(m_pendingReplyMessages.empty()) { return LSP::RequestMessage::Ptr_t(nullptr); }
    if(m_pendingReplyMessages.count(msgid) == 0) { return LSP::RequestMessage::Ptr_t(nullptr); }
    LSP::RequestMessage::Ptr_t msgptr = m_pendingReplyMessages[msgid];
    m_pendingReplyMessages.erase(msgid);
    return msgptr;
}