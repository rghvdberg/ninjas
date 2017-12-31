## just some notes so I won't forget 

>basically add this function:
void uiFileBrowserSelected(const char* filename) override { /* stuff */ }
this is async, so your plugin will continue working while the file picker/browser is active
that function is called by DPF when a file is selected
to open/start the dialog, see https://github.com/zamaudio/zam-plugins/blob/master/plugins/ZamSFZ/ZamSFZUI.cpp#L115
zamaudio/zam-plugins
 
