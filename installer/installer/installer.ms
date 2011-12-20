global edgeSmoothInstaller;

rollout edgesmooth_install ""
(
   edittext progress_txt height:200 readonly:true;
   button close_btn "Close" enabled:false width:273 align:#center    
   on close_btn pressed do
      destroyDialog edgesmooth_install;
)


struct edgeSmooth_installer (

tempDir             = (GetDir #temp) + "/edgesmooth",
userIconsDir        = GetDir #userIcons,
userDir             = pathConfig.removePathLeaf (getdir #userscripts),
pluginDir           = userDir + "\\plugins\\edgesmooth",
pluginUserSettings  = userDir + "\\Plugin.UserSettings.ini",
strings_file        = tempDir + "/installer/strings.ini",
installer_data_file = tempDir + "/installer/installer_data.ini",

--Retrieves the string from the strings ini file.
function getString key section:#Strings = 
(
   local str = getINISetting strings_file section key;
   
   if (str == undefined) do 
   (
      print ("Outliner installer: cannot find string " + key + " in section " + section);
      str = "";
   )
   
   --Replace escaped newline characters and return result.
   substituteString str "\\n" "\n";
),


--Returns the version of this installer.
function getVersion asString:false =
(
   if (not asString) then
      (getINISetting installer_data_file #Version #version) as float;
   else
      getINISetting installer_data_file #Version #versionString
),




--Prints an exception message to the maxscript listener.
function printException executingFn exception =
(
   format "** EdgeSmooth Plugin installer exception **\nIn function: %\n%\n" \
          executingFn \
          exception;
),


--Adds a post-system-shutdown callback that removes the temporary files.
function clearTempDir =
(
   local scriptStr = ("local d = (GetDir #temp) + \"/edgesmooth\";
      if ((dotNetClass \"System.IO.Directory\").Exists d) do 
         try ( (dotNetClass \"System.IO.Directory\").Delete d true; ) catch ()");
   
   callbacks.removeScripts #preSystemShutdown id:#edgesmooth_installer;
   callbacks.addScript #preSystemShutdown scriptStr id:#edgesmooth_installer;
),





-----------------------------------------------------------------------------
--PREREQUISITES

function isPluginInstalled =
(
   local pluginFile = pathConfig.appendPath pluginDir "edgesmooth.gup";
   EdgeSmooth != undefined OR doesFileExist pluginFile;
),

--Returns the plugin file for this 3dsmax version.
function getPluginFile =
(
   local file = tempDir + "/plugin/max";
   file += (1998 + (maxVersion())[1] / 1000) as string;
   if (is64bitApplication()) then
      file += "_x64";
   else
      file += "_win32";
      
   file + "/EdgeSmooth.gup";
),

function checkMaxVersion =
(
   doesFileExist (getPluginFile());
),





-----------------------------------------------------------------------------
-- UI FUNCTIONS

function openInstallWindow height =
(
   try ( destroyDialog edgesmooth_install ) catch ()
   
   createDialog edgesmooth_install 300 \
                                   height \
                                   lockWidth:true \
                                   lockHeight:true \
                                   style:#(#style_sysmenu, #style_toolwindow)
   
   edgesmooth_install.progress_txt.height = height - 40;
   edgesmooth_install.close_btn.pos.y = height - 28;
),



function setStatusTitle txt =
(
   if (edgesmooth_install.open) do
      edgesmooth_install.title = txt;
),

function addStatusText txt newline:true =
(
   if (edgesmooth_install.open) do
   (
      if (newline AND edgesmooth_install.progress_txt.text != "") do 
         txt = "\n" + txt;
      
      edgesmooth_install.progress_txt.text += txt;
   )
),



function success title msg =
(
   local inst_txt = (getString title);
   
   addStatusText "";
   addStatusText inst_txt;
   
   edgesmooth_install.close_btn.enabled = true;
   
   local mb     = dotNetClass "System.Windows.Forms.MessageBox";
   local mbBtn  = dotNetClass "System.Windows.Forms.MessageBoxButtons";
   local mbIcon = dotNetClass "System.Windows.Forms.MessageBoxIcon";
   mb.Show (getString msg) \
           (getString title)\
           mbBtn.OK \
           mbIcon.Information;
),


function failed inst msg silent:false =
(
   if (not silent) do
   (
      addStatusText ("\n\n" + inst + " " + (getString #failed));
      edgesmooth_install.close_btn.enabled = true;
      messageBox msg title:(getString #error);
   )
),




-----------------------------------------------------------------------------
--ICONS

function copyIcons =
(
   local result = true;
   
   try
   (
      local iconFiles = getFiles (tempDir + "/icons/*.bmp");
      for icon in iconFiles do
      (
         local targetFile = pathConfig.appendPath userIconsDir (filenameFromPath icon);
         if (doesFileExist  targetFile) do
            deleteFile targetFile;
         
         copyFile icon targetFile;
      )
   )
   catch
   (
      printException "copyIcons" (getCurrentException());
      result = false;
   )
   
   result;
),

function removeIcons =
(
   local result = true;
   
   iconFiles = getFiles (userIconsDir + "/edgesmooth_*");
   for ico in iconFiles do
   (
      if (not (deleteFile ico)) do 
         result = false;
   )
   
   result;
),




-----------------------------------------------------------------------------
--PLUGIN

function copyPlugin =
(
   local result = true;
   try
   (
      setINISetting pluginUserSettings #Directories #EdgeSmooth pluginDir;
      
      local pluginFile = getPluginFile();
      local targetFile = (pluginDir + "/" + (filenameFromPath pluginFile));
      makeDir pluginDir;
      
      if (doesFileExist targetFile) do
      (
         if (not (deleteFile targetFile)) do
            return false;
      )
      
      copyFile pluginFile targetFile;
   )
   catch
   (
      printException "copyPlugin" (getCurrentException());
      result = false;
   )
   
   result;
),

function removePlugin =
(
   local result = true;
   try
   (
   delINISetting pluginUserSettings #Directories #EdgeSmooth;
   
   local readme = openFile (pluginDir + "\\README.txt") mode:"wt";
   format "This directory will be deleted the next time 3dsmax is started.\r\n" to:readme;
   format "If it isn't, close 3dsmax and remove it manually." to:readme;
   close readme;
   
   local srcRemPluginScript = pathConfig.appendPath tempDir \
                                                    "installer\\remove_edgesmooth_plugin.ms"
   local tgtRemPluginScript = pathConfig.appendPath (GetDir #userStartupScripts) \
                                                    "remove_edgesmooth_plugin.ms";
   if (doesFileExist tgtRemPluginScript) do
      deleteFile tgtRemPluginScript;
   
   copyFile srcRemPluginScript tgtRemPluginScript;
   )
   catch
   (
      printException "removePlugin" (getCurrentException());
      result = false;
   )
   
   result;
),



function loadPlugin =
(
   colorman.reInitIcons();
   loadDllsFromDir (pluginDir + "\\") "*.gup";
   scanForNewPlugins();
),





-----------------------------------------------------------------------------
--INSTALL & UNINSTALL

function uninstall =
(
   openInstallWindow 140;
   setStatusTitle ((getString #title) + " " + (getString #uninst));
   addStatusText (getString #begUnInst);
   
   
   addStatusText (getString #remIcons);
   if (removeIcons()) then
      addStatusText (getString #done) newline:false;
   else
   (
      addStatusText (getString #failed) newline:false;
      failed (getString #inst) \
             (getString #remIcons section:#Error);
      return false;
   )
   
   
   
   addStatusText (getString #remPlugin);
   if (removePlugin()) then
      addStatusText (getString #done) newline:false;
   else
   (
      addStatusText (getString #failed) newline:false;
      failed (getString #inst) \
             (getString #remPlugin section:#Error);
      return false;
   )
 
   
   
   success #uninstSuccess #uninstSuccessMsg;
   
   clearTempDir();
   
   true;
),










function install =
(
   openInstallWindow 180;
   setStatusTitle ((getString #title) + " " + (getString #inst));
   addStatusText (getString #begInst);
   
   addStatusText (getString #checkReq);
   if (checkMaxVersion()) then
      addStatusText (getString #done) newline:false;
   else
   (
      addStatusText (getString #failed) newline:false;
      failed (getString #inst) \
             (getString #maxVersion section:#Error);
      return false;
   )
   
      
   
   addStatusText (getString #copyIcons);
   if (copyIcons()) then
      addStatusText (getString #done) newline:false;
   else
   (
      addStatusText (getString #failed) newline:false;
      failed (getString #inst) \
             (getString #copyIcons section:#Error);
      return false;
   )
   
   
   addStatusText (getString #copyPlugin);
   if (copyPlugin()) then
      addStatusText (getString #done) newline:false;
   else
   (
      addStatusText (getString #failed) newline:false;
      failed (getString #inst) \
             (getString #copyPlugin section:#Error);
      return false;
   )
   
   
   loadPlugin();

         
   success #instSuccess #instSuccessMsg;
   
   clearTempDir();
   
   true;
)

)


edgeSmoothInstaller = edgeSmooth_installer();