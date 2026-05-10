#include "PDFGenerator.h"
#include <fstream>
#include <cstdlib>
#include <iostream>

void makePdf(std::string filename, std::string title, std::string contentString)
{
    std::string htmlFilename = filename;
    size_t dotPos = htmlFilename.find_last_of('.');
    if (dotPos != std::string::npos)
    {
        htmlFilename = htmlFilename.substr(0, dotPos) + ".html";
    }
    else
    {
        htmlFilename += ".html";
    }
    // HTML File created here 
    std::ofstream html(htmlFilename);

    html << "<html><head><style>"
         << "body { font-family: 'Courier New', Courier, monospace; margin: 50px; }"
         << "h2 { color: #2c3e50; border-bottom: 1px solid #ccc; }"
         << ".content { white-space: pre-wrap; font-size: 14px; line-height: 1.5; }"
         << "</style></head><body>"
         << "<h2>" << title << "</h2>"
         << "<div class='content'>" << contentString << "</div>"
         << "</body></html>";

    html.close();

    std::cout << "Report saved as: " << htmlFilename << "\n";

    // system command to open in browser directly
    std::string command = "start " + htmlFilename;
    std::system(command.c_str());
}