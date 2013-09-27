//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "sftp_ui.h"
#include "codelite_exports.h"


// Declare the bitmap loading function
extern void wxCE8CInitBitmapResources();

static bool bBitmapLoaded = false;


SSHAccountManagerDlgBase::SSHAccountManagerDlgBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCE8CInitBitmapResources();
        bBitmapLoaded = true;
    }
    // Set icon(s) to the application/dialog
    wxIconBundle app_icons;
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("ssh-16"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("ssh-32"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    SetIcons( app_icons );

    
    wxBoxSizer* boxSizer2 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer2);
    
    wxBoxSizer* boxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer2->Add(boxSizer10, 1, wxALL|wxEXPAND, 5);
    
    m_dvListCtrlAccounts = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(200,-1), wxDV_ROW_LINES|wxDV_MULTIPLE|wxDV_SINGLE);
    
    boxSizer10->Add(m_dvListCtrlAccounts, 1, wxALL|wxEXPAND, 5);
    
    m_dvListCtrlAccounts->AppendTextColumn(_("Host"), wxDATAVIEW_CELL_INERT, -2, wxALIGN_LEFT);
    m_dvListCtrlAccounts->AppendTextColumn(_("User"), wxDATAVIEW_CELL_INERT, -2, wxALIGN_LEFT);
    wxBoxSizer* boxSizer15 = new wxBoxSizer(wxVERTICAL);
    
    boxSizer10->Add(boxSizer15, 0, wxEXPAND, 5);
    
    m_buttonNew = new wxButton(this, wxID_ADD, _("&Add"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer15->Add(m_buttonNew, 0, wxALL|wxEXPAND, 5);
    
    m_buttonDelete = new wxButton(this, wxID_DELETE, _("&Delete"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer15->Add(m_buttonDelete, 0, wxALL|wxEXPAND, 5);
    
    wxBoxSizer* boxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer2->Add(boxSizer4, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
    
    m_button6 = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_button6->SetDefault();
    
    boxSizer4->Add(m_button6, 0, wxALL, 5);
    
    m_button8 = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer4->Add(m_button8, 0, wxALL, 5);
    
    SetSizeHints(-1,-1);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);
    // Connect events
    m_dvListCtrlAccounts->Connect(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler(SSHAccountManagerDlgBase::OnEditAccount), NULL, this);
    m_buttonNew->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SSHAccountManagerDlgBase::OnAddAccount), NULL, this);
    m_buttonDelete->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SSHAccountManagerDlgBase::OnDeleteAccount), NULL, this);
    m_buttonDelete->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SSHAccountManagerDlgBase::OnDeleteAccountUI), NULL, this);
    
}

SSHAccountManagerDlgBase::~SSHAccountManagerDlgBase()
{
    m_dvListCtrlAccounts->Disconnect(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler(SSHAccountManagerDlgBase::OnEditAccount), NULL, this);
    m_buttonNew->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SSHAccountManagerDlgBase::OnAddAccount), NULL, this);
    m_buttonDelete->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SSHAccountManagerDlgBase::OnDeleteAccount), NULL, this);
    m_buttonDelete->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SSHAccountManagerDlgBase::OnDeleteAccountUI), NULL, this);
    
}

AddSSHAcountDlgBase::AddSSHAcountDlgBase(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCE8CInitBitmapResources();
        bBitmapLoaded = true;
    }
    // Set icon(s) to the application/dialog
    wxIconBundle app_icons;
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("ssh-16"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    {
        wxBitmap iconBmp = wxXmlResource::Get()->LoadBitmap(wxT("ssh-32"));
        wxIcon icn;
        icn.CopyFromBitmap(iconBmp);
        app_icons.AddIcon( icn );
    }
    SetIcons( app_icons );

    
    wxBoxSizer* boxSizer23 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer23);
    
    wxFlexGridSizer* flexGridSizer31 = new wxFlexGridSizer(  0, 2, 0, 0);
    flexGridSizer31->SetFlexibleDirection( wxBOTH );
    flexGridSizer31->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
    flexGridSizer31->AddGrowableCol(1);
    
    boxSizer23->Add(flexGridSizer31, 0, wxALL|wxEXPAND, 5);
    
    m_staticText98 = new wxStaticText(this, wxID_ANY, _("Account Name:"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_staticText98->SetToolTip(_("Give this account a unique name"));
    
    flexGridSizer31->Add(m_staticText98, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    m_textCtrlName = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), 0);
    m_textCtrlName->SetToolTip(_("Give this account a unique name"));
    m_textCtrlName->SetFocus();
    
    flexGridSizer31->Add(m_textCtrlName, 0, wxALL|wxEXPAND, 5);
    
    m_staticHostIp = new wxStaticText(this, wxID_ANY, _("Host / IP:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer31->Add(m_staticHostIp, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    m_textCtrlHost = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), 0);
    m_textCtrlHost->SetToolTip(_("The remote host ip address or its known name"));
    
    flexGridSizer31->Add(m_textCtrlHost, 0, wxALL|wxEXPAND, 5);
    
    m_staticText45 = new wxStaticText(this, wxID_ANY, _("Port:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer31->Add(m_staticText45, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    m_textCtrlPort = new wxTextCtrl(this, wxID_ANY, wxT("22"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_textCtrlPort->SetToolTip(_("The SSH port. If you don't know it, leave it as 22 (SSH default port)"));
    
    flexGridSizer31->Add(m_textCtrlPort, 0, wxALL|wxEXPAND, 5);
    
    m_staticText37 = new wxStaticText(this, wxID_ANY, _("Username:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer31->Add(m_staticText37, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    m_textCtrlUsername = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), 0);
    m_textCtrlUsername->SetToolTip(_("Login user name"));
    
    flexGridSizer31->Add(m_textCtrlUsername, 0, wxALL|wxEXPAND, 5);
    
    m_staticText41 = new wxStaticText(this, wxID_ANY, _("Password:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    flexGridSizer31->Add(m_staticText41, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    m_textCtrlPassword = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), wxTE_PASSWORD);
    m_textCtrlPassword->SetToolTip(_("The login password"));
    
    flexGridSizer31->Add(m_textCtrlPassword, 0, wxALL|wxEXPAND, 5);
    
    m_button51 = new wxButton(this, wxID_ANY, _("Test Connection"), wxDefaultPosition, wxSize(-1,-1), 0);
    #if wxVERSION_NUMBER >= 2904
    m_button51->SetBitmap(wxXmlResource::Get()->LoadBitmap(wxT("connect")));
    m_button51->SetBitmapMargins(5,5);
    #endif
    
    boxSizer23->Add(m_button51, 0, wxALL|wxEXPAND, 10);
    
    boxSizer23->Add(0, 0, 1, wxALL, 5);
    
    wxBoxSizer* boxSizer29 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer23->Add(boxSizer29, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
    
    m_button27 = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_button27->SetDefault();
    
    boxSizer29->Add(m_button27, 0, wxALL, 5);
    
    m_button25 = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer29->Add(m_button25, 0, wxALL, 5);
    
    SetSizeHints(-1,-1);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);
    // Connect events
    m_button51->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AddSSHAcountDlgBase::OnTestConnection), NULL, this);
    m_button51->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(AddSSHAcountDlgBase::OnTestConnectionUI), NULL, this);
    m_button27->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(AddSSHAcountDlgBase::OnOKUI), NULL, this);
    
}

AddSSHAcountDlgBase::~AddSSHAcountDlgBase()
{
    m_button51->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AddSSHAcountDlgBase::OnTestConnection), NULL, this);
    m_button51->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(AddSSHAcountDlgBase::OnTestConnectionUI), NULL, this);
    m_button27->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(AddSSHAcountDlgBase::OnOKUI), NULL, this);
    
}

SFTPBrowserBaseDlg::SFTPBrowserBaseDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style)
{
    if ( !bBitmapLoaded ) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxCE8CInitBitmapResources();
        bBitmapLoaded = true;
    }
    
    wxBoxSizer* boxSizer62 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer62);
    
    wxGridBagSizer* gridBagSizer80 = new wxGridBagSizer(0, 0);
    
    boxSizer62->Add(gridBagSizer80, 0, wxALL|wxEXPAND, 5);
    
    m_staticText66 = new wxStaticText(this, wxID_ANY, _("Path:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    gridBagSizer80->Add(m_staticText66, wxGBPosition(1,0), wxGBSpan(1,1), wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    m_textCtrlRemoteFolder = new wxTextCtrl(this, wxID_ANY, wxT("/"), wxDefaultPosition, wxSize(-1,-1), wxTE_PROCESS_ENTER);
    m_textCtrlRemoteFolder->SetToolTip(_("Set the remote folder to browse and click on the 'Refresh' button"));
    m_textCtrlRemoteFolder->SetFocus();
    
    gridBagSizer80->Add(m_textCtrlRemoteFolder, wxGBPosition(1,1), wxGBSpan(1,1), wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5);
    
    m_buttonRefresh = new wxButton(this, wxID_REFRESH, _("Connect..."), wxDefaultPosition, wxSize(-1,-1), 0);
    
    gridBagSizer80->Add(m_buttonRefresh, wxGBPosition(1,2), wxGBSpan(1,1), wxALL|wxALIGN_CENTER_VERTICAL, 5);
    
    m_staticText82 = new wxStaticText(this, wxID_ANY, _("Account:"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    gridBagSizer80->Add(m_staticText82, wxGBPosition(0,0), wxGBSpan(1,1), wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    
    wxArrayString m_choiceAccountArr;
    m_choiceAccount = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), m_choiceAccountArr, 0);
    
    gridBagSizer80->Add(m_choiceAccount, wxGBPosition(0,1), wxGBSpan(1,2), wxALL|wxEXPAND, 5);
    gridBagSizer80->AddGrowableCol(1);
    m_toolbar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1,-1), wxTB_NODIVIDER|wxTB_FLAT);
    m_toolbar->SetToolBitmapSize(wxSize(16,16));
    
    boxSizer62->Add(m_toolbar, 0, wxALL|wxEXPAND, 5);
    
    m_toolbar->AddTool(ID_CD_UP, _("Parent folder"), wxXmlResource::Get()->LoadBitmap(wxT("arrow-up")), wxNullBitmap, wxITEM_NORMAL, _("Parent folder"), _("Parent folder"), NULL);
    
    m_toolbar->AddTool(ID_SSH_ACCOUNT_MANAGER, _("Open SSH Account Manager..."), wxXmlResource::Get()->LoadBitmap(wxT("ssh-16")), wxNullBitmap, wxITEM_NORMAL, _("Open SSH Account Manager..."), _("Open SSH Account Manager..."), NULL);
    m_toolbar->Realize();
    
    m_dataview = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(600,100), wxDV_ROW_LINES|wxDV_SINGLE);
    
    m_dataviewModel = new SFTPTreeModel;
    m_dataviewModel->SetColCount( 3 );
    m_dataview->AssociateModel(m_dataviewModel.get() );
    
    boxSizer62->Add(m_dataview, 1, wxALL|wxEXPAND, 5);
    
    m_dataview->AppendIconTextColumn(_("Name"), m_dataview->GetColumnCount(), wxDATAVIEW_CELL_INERT, 400, wxALIGN_LEFT);
    m_dataview->AppendTextColumn(_("Type"), m_dataview->GetColumnCount(), wxDATAVIEW_CELL_INERT, 100, wxALIGN_LEFT);
    m_dataview->AppendTextColumn(_("Size"), m_dataview->GetColumnCount(), wxDATAVIEW_CELL_INERT, 100, wxALIGN_LEFT);
    wxBoxSizer* boxSizer94 = new wxBoxSizer(wxVERTICAL);
    
    boxSizer62->Add(boxSizer94, 0, wxEXPAND|wxRESERVE_SPACE_EVEN_IF_HIDDEN, 5);
    
    m_textCtrlInlineSearch = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(-1,-1), wxTE_PROCESS_ENTER);
    m_textCtrlInlineSearch->Hide();
    
    boxSizer94->Add(m_textCtrlInlineSearch, 0, wxALL|wxEXPAND, 5);
    
    wxBoxSizer* boxSizer57 = new wxBoxSizer(wxHORIZONTAL);
    
    boxSizer62->Add(boxSizer57, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5);
    
    m_button59 = new wxButton(this, wxID_OK, _("&OK"), wxDefaultPosition, wxSize(-1,-1), 0);
    m_button59->SetDefault();
    
    boxSizer57->Add(m_button59, 0, wxALL, 5);
    
    m_button61 = new wxButton(this, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxSize(-1,-1), 0);
    
    boxSizer57->Add(m_button61, 0, wxALL, 5);
    
    SetSizeHints(-1,-1);
    if ( GetSizer() ) {
         GetSizer()->Fit(this);
    }
    Centre(wxBOTH);
    // Connect events
    m_textCtrlRemoteFolder->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SFTPBrowserBaseDlg::OnTextEnter), NULL, this);
    m_buttonRefresh->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SFTPBrowserBaseDlg::OnRefresh), NULL, this);
    m_buttonRefresh->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SFTPBrowserBaseDlg::OnRefreshUI), NULL, this);
    this->Connect(ID_CD_UP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SFTPBrowserBaseDlg::OnCdUp), NULL, this);
    this->Connect(ID_CD_UP, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SFTPBrowserBaseDlg::OnCdUpUI), NULL, this);
    this->Connect(ID_SSH_ACCOUNT_MANAGER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SFTPBrowserBaseDlg::OnSSHAccountManager), NULL, this);
    m_dataview->Connect(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler(SFTPBrowserBaseDlg::OnItemActivated), NULL, this);
    m_dataview->Connect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(SFTPBrowserBaseDlg::OnItemSelected), NULL, this);
    m_dataview->Connect(wxEVT_CHAR, wxKeyEventHandler(SFTPBrowserBaseDlg::OnKeyDown), NULL, this);
    m_textCtrlInlineSearch->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SFTPBrowserBaseDlg::OnTextUpdated), NULL, this);
    m_textCtrlInlineSearch->Connect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SFTPBrowserBaseDlg::OnEnter), NULL, this);
    m_textCtrlInlineSearch->Connect(wxEVT_KILL_FOCUS, wxFocusEventHandler(SFTPBrowserBaseDlg::OnFocusLost), NULL, this);
    m_button59->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SFTPBrowserBaseDlg::OnOKUI), NULL, this);
    
}

SFTPBrowserBaseDlg::~SFTPBrowserBaseDlg()
{
    m_textCtrlRemoteFolder->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SFTPBrowserBaseDlg::OnTextEnter), NULL, this);
    m_buttonRefresh->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SFTPBrowserBaseDlg::OnRefresh), NULL, this);
    m_buttonRefresh->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SFTPBrowserBaseDlg::OnRefreshUI), NULL, this);
    this->Disconnect(ID_CD_UP, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SFTPBrowserBaseDlg::OnCdUp), NULL, this);
    this->Disconnect(ID_CD_UP, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SFTPBrowserBaseDlg::OnCdUpUI), NULL, this);
    this->Disconnect(ID_SSH_ACCOUNT_MANAGER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SFTPBrowserBaseDlg::OnSSHAccountManager), NULL, this);
    m_dataview->Disconnect(wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler(SFTPBrowserBaseDlg::OnItemActivated), NULL, this);
    m_dataview->Disconnect(wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(SFTPBrowserBaseDlg::OnItemSelected), NULL, this);
    m_dataview->Disconnect(wxEVT_CHAR, wxKeyEventHandler(SFTPBrowserBaseDlg::OnKeyDown), NULL, this);
    m_textCtrlInlineSearch->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(SFTPBrowserBaseDlg::OnTextUpdated), NULL, this);
    m_textCtrlInlineSearch->Disconnect(wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(SFTPBrowserBaseDlg::OnEnter), NULL, this);
    m_textCtrlInlineSearch->Disconnect(wxEVT_KILL_FOCUS, wxFocusEventHandler(SFTPBrowserBaseDlg::OnFocusLost), NULL, this);
    m_button59->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SFTPBrowserBaseDlg::OnOKUI), NULL, this);
    
}
