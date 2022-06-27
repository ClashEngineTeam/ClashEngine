class Program
{
    public static void Main(string[] args)
    {
        string vcxprojInputFilePath = string.Empty;
        string hppFileOutputFolderPath = string.Empty;
        string cppFileOutputFolderPath = string.Empty;
        const string ClInclude = "<ClInclude ";
        const string ClCompile = "<ClCompile ";
        if (args.Length == 3)
        {
            vcxprojInputFilePath = args[0];
            hppFileOutputFolderPath = args[1];
            cppFileOutputFolderPath = args[2];
        }
        else
        {
            vcxprojInputFilePath = @"C:\Users\16692\Desktop\freetype.vcxproj";
            hppFileOutputFolderPath = @"C:\Users\16692\source\ClashEngineTeam\ClashEngine\ClashEngine\FreeType\include\";
            cppFileOutputFolderPath = @"C:\Users\16692\source\ClashEngineTeam\ClashEngine\ClashEngine\FreeType\";
        }
        if (File.Exists(vcxprojInputFilePath))
        {
            string[] lines = File.ReadAllLines(vcxprojInputFilePath);
            Array.ForEach(lines, line =>
            {
                if (line.IndexOf(ClInclude) != -1)
                {
                    int startPos = line.IndexOf("\"");
                    int endPos = line.LastIndexOf("\"");
                    string path = line.Substring(startPos + 1, endPos - startPos - 1);
                    string destPath = Path.Combine(hppFileOutputFolderPath, new FileInfo(path).Name);
                    File.Copy(path, destPath, true);
                }
                if (line.IndexOf(ClCompile) != -1)
                {
                    int startPos = line.IndexOf("\"");
                    int endPos = line.LastIndexOf("\"");
                    string path = line.Substring(startPos + 1, endPos - startPos - 1);
                    string destPath = Path.Combine(cppFileOutputFolderPath, new FileInfo(path).Name);
                    File.Copy(path, destPath, true);
                }
            });
        }
    }
}