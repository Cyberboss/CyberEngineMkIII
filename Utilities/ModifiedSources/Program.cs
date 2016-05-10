using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace ModifiedSources
{
    class Program
    {
        class WriteJob
        {
            public FileStream FRead, FWrite;
        }
        static List<Task> FTasks = new List<Task>();
        static int FCopied = 0;
        static List<string> extensions = new List<string> { ".exe", ".obj", ".pch", ".pdb", ".dll", ".lib", ".rc", ".vcxproj", ".filters", ".user", ".tlog", ".log", ".idb", ".xml", ".lastbuildstate", ".pchast", ".exp", ".ilk", ".lastcodeanalysissucceeded", ".CYB", ".csproj", ".cs", ".cache", ".ipch" , "unsuccessfulbuild", ".sln", ".sdf", ".opensdf", ".config", ".settings", ".res", ".opendb", ".db", ".asm", ".sbr", ".bsc", ".xaml", ".cache", ".ipdb", ".iobj", ".tmp", ".ps1", ".filters"};

       static DateTime from_date = Properties.Settings.Default.LastSync;
        static void ParseDirectory(DirectoryInfo ADir, String CurrentPath)
        {
            Directory.CreateDirectory(CurrentPath);
            foreach (var Dir in ADir.GetDirectories())
                if (ADir.Name == "SupportLibs" && Dir.Name == "Catch")
                {
                    var NewPath = CurrentPath + Path.DirectorySeparatorChar + Dir.Name + Path.DirectorySeparatorChar;
                    Directory.CreateDirectory(CurrentPath + Path.DirectorySeparatorChar + Dir.Name);
                    Directory.CreateDirectory(NewPath + "single_include");
                    Directory.CreateDirectory(NewPath + "include");
                    ParseDirectory(new DirectoryInfo(Dir.FullName + Path.DirectorySeparatorChar + "single_include"), NewPath + "single_include");
                    ParseDirectory(new DirectoryInfo(Dir.FullName + Path.DirectorySeparatorChar + "include"), NewPath + "include");
                }
                else if (Dir.Name != ".git" && Dir.Name != ".vs" && Dir.Name != "Documentation" && Dir.Name != "GDBSync")
                    ParseDirectory(Dir, CurrentPath + Path.DirectorySeparatorChar + Dir.Name);
            
            var actualfiles = ADir.GetFiles();
            var files = ADir.GetFiles()
              .Where(file => file.LastWriteTime >= from_date &&
              !extensions.Any(file.Extension.Contains));
            foreach (var file in files)
            {
                WriteJob J = new WriteJob();
                J.FRead = file.Open(FileMode.Open);
                J.FWrite = File.Open(CurrentPath + Path.DirectorySeparatorChar + file.Name, FileMode.Create);
                FTasks.Add(J.FRead.CopyToAsync(J.FWrite));

                ++FCopied;
            }
        }
        static void Main(string[] args)
        {
            foreach (var a in args)
                if(a == "--clear")
                {
                    Properties.Settings.Default.LastSync = new DateTime(1,1,1);
                    Properties.Settings.Default.Save();
                    Console.WriteLine("Settings cleared");
                    return;
                }
            try
            {
                Directory.Delete("./GDBSync", true);
            }
            catch { }
            Console.WriteLine("Getting modified sources. Please wait...");
            Console.WriteLine("Start Directory: " + new DirectoryInfo("./").FullName);
            ParseDirectory(new DirectoryInfo("./"), "./GDBSync");

            foreach (Task t in FTasks)
                t.Wait();

            Properties.Settings.Default.LastSync = DateTime.Now;
            Properties.Settings.Default.Save();
            if (FCopied == 0)
            {
                Console.WriteLine("No changes");
                Environment.Exit(1);
            }
        }
    }
}
