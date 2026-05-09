#include "PDFGenerator.h"
#include <fstream>
#include <cstdlib>
#include <iostream>

void makePdf(std::string filename, std::string title, std::string contentString)
{
    // SAFETY CHECK: Force the filename to end in .pdf 
    if (filename.length() < 4 || filename.substr(filename.length() - 4) != ".pdf")
    {
        filename += ".pdf";
    }

    // Determine the HTML filename based on the PDF filename
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

    // Create and write the HTML blueprint
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

    // --- PDF CONVERSION LOGIC ---

    // 1. Convert the HTML to PDF using the bundled tool
    std::string pdfCommand = "..\\tools\\wkhtmltopdf.exe --quiet " + htmlFilename + " " + filename;
    std::system(pdfCommand.c_str());

    std::cout << "Report successfully converted to PDF: " << filename << "\n";

    // 2. Open the PDF directly in the default Windows viewer
    std::string openCommand = "start " + filename;
    std::system(openCommand.c_str());

    // 3. Delete the temporary HTML file to keep the folder clean
    std::remove(htmlFilename.c_str());
}