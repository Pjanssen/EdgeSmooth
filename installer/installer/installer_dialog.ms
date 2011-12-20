fileIn "installer.ms";

/**
 * PRE INSTALL DIALOG (CHOOSE INSTALL / UNINSTALL)
 */
rollout edgesmooth_pre_install ((edgeSmoothInstaller.getString #title) + " " + \
                              (edgeSmoothInstaller.getVersion asString:true) + \
                              " installation")
(
   label title_lbl "" offset:[0, 5];
   label descr_lbl "" offset:[0, 5];
   label descr_lbl2 "" offset:[0, -2];
   button install_btn "Install" width:60 offset:[0, 10] across:3
   button uninstall_btn "Uninstall" width:60 offset:[0, 10]
   button close_btn "Close" width:60 offset:[0, 10]
   
   on edgesmooth_pre_install open do
   (
      local isInstalled = edgeSmoothInstaller.isPluginInstalled();
      
      title_lbl.text = (edgeSmoothInstaller.getString #title) + " " + \
                       (edgeSmoothInstaller.getVersion asString:true);
	   descr_lbl.text = edgeSmoothInstaller.getString #descr_lbl1;
	   
	   install_btn.enabled = not isInstalled;
	   uninstall_btn.enabled = isInstalled;
	   
	   if (isInstalled) then
	      descr_lbl2.text = edgeSmoothInstaller.getString #descr_lbl2;
   )
   
   on install_btn pressed do 
   (
      destroyDialog edgesmooth_pre_install;
      edgeSmoothInstaller.install();
   )
   
   on uninstall_btn pressed do
   (
      destroyDialog edgesmooth_pre_install;
      edgeSmoothInstaller.uninstall();
   )
   
   on close_btn pressed do
   (
      destroyDialog edgesmooth_pre_install;
      edgeSmoothInstaller.clearTempDir();
   )
)--End rollout.


createDialog edgesmooth_pre_install 330 \
                                    110 \
                                    modal:true \
                                    lockWidth:true \
                                    lockHeight:true \
                                    style:#(#style_sysmenu, #style_toolwindow);