--This is a temporary script used to remove the edgesmooth plugin from 
--3dsmax the next time it is started. After that max session ends, it 
--will delete itself.
--Should it remain, you can delete it manually. In that case also check 
--if [userdir]\plugins\edgesmooth was deleted. If not, close 3dsmax and
--delete the directory manually.

try 
(
   local userDir = pathConfig.removePathLeaf (getdir #userscripts);
   local pluginDir = pathConfig.appendPath userDir "plugins\\edgesmooth";
   
   if (doesFileExist pluginDir) do
   (
      local rmDirCmd = "rmdir /Q /S \"" + pluginDir + "\"";
      hiddendoscommand rmDirCmd startpath:"$max"
   )
   
   --We can only remove this file when it is no longer executing.
   --So we do that in a pre-system-shutdown callback.
   local str = "deleteFile (pathConfig.appendPath (GetDir #userStartupScripts) \"remove_edgesmooth_plugin.ms\")";
   callbacks.removeScripts #preSystemShutdown id:#edgesmooth_installer;
   callbacks.addScript #preSystemShutdown str id:#edgesmooth_installer;
)
catch 
(
   format "Failed to remove EdgeSmooth plugin:\n";
   format "%\n" (getCurrentException());
)