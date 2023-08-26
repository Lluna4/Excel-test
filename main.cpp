#include "db.hpp"
//#include <external/nowide/nowide/iostream.hpp>

using namespace OpenXLSX;
std::vector<std::string> header;

std::vector<db> dbs;

void print_line(std::vector<std::string> header)
{
    for (unsigned int i = 0; i < header.size(); i++)
    {
        std::cout << "+";
        std::cout << "-";
        for (unsigned int x = 0; x < header[i].size(); x++)
            std::cout << "-";
        std::cout << "-";
    }
    std::cout << "+";
    std::cout << std::endl;
}

void print_line(std::vector<std::string> header, std::vector<int> max)
{
    for (unsigned int i = 0; i < header.size(); i++)
    {
        std::cout << "+";
        std::cout << "-";
        for (unsigned int x = 0; x < max[i]; x++)
            std::cout << "-";
        std::cout << "-";
    }
    std::cout << "+";
    std::cout << std::endl;
}

void print_words(std::vector<std::string> header)
{
    for (unsigned int i = 0; i < header.size(); i++)
    {
        std::cout << "|" << " " << header[i] << " ";
    }
    std::cout << "|";
    std::cout << std::endl;
}

void print_words(std::vector<std::string> header, std::vector<int> max)
{
    for (unsigned int i = 0; i < header.size(); i++)
    {
        std::cout << "|" << " " << header[i];
        for (unsigned int x = 0; x < (max[i] - (header[i].length() - 1)); x++)
            std::cout << " ";
    }
    std::cout << "|";
    std::cout << std::endl;
}

void show()
{
    std::vector<int> max;
    std::vector<std::vector<std::string>> values = dbs[0].get_values();
    std::vector<std::string> header = dbs[0].get_header();
    for (unsigned int y = 0; y < header.size(); y++)
    {
        max.push_back(header[y].length());
        for (unsigned int x = 0; x < values.size(); x++)
        {
            if (max.empty() || (max.size() - 1) < y)
            {
                max.push_back(values[x][y].size());
                continue;
            }
            if (values[x][y].size() > max[y])
                max[y] = values[x][y].size();
        }
    }
    print_line(header, max);
    print_words(header, max);
    print_line(header, max);
    for (unsigned int y = 0; y < values.size(); y++)
    {
        print_words(values[y], max);
        print_line(values[y], max);
    }

    return;
}


int main() 
{

    XLDocument doc;
    XLDocument write;
    write.create("output.xlsx");
    doc.open("test.xlsx");
    write.workbook().addWorksheet("EQUIDAD");
    auto wkb = write.workbook().worksheet("EQUIDAD");
    auto wks = doc.workbook().worksheet("Hoja1");
    std::cout << wks.rowCount();
    //db_writee.close();
    write.save();
    uint16_t col = 1;
    uint32_t row = 1;
    for (uint32_t i = 1; i < 1000; i++)
    {
        col = 1;
        auto rows = wks.row(i);
        auto cells = rows.cells();
        if (i < 14)
            continue;
        if (i == 14)
        {
            for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
            {
                if (ptr->value().operator OpenXLSX::XLCellValue().get<std::string>().compare("ITEM") == 0)
                    std::cout << ptr->value().typeAsString() << ptr->value().operator OpenXLSX::XLCellValue() << std::endl;
                if (ptr->value().typeAsString().compare("empty") == 0)
                    continue;
                header.push_back(ptr->value().operator OpenXLSX::XLCellValue().get<std::string>());
                //std::cout << ptr->value().typeAsString() << ptr->value().operator OpenXLSX::XLCellValue() << std::endl;
            }
            dbs.push_back(db("tabla", header));
        }
        int a = dbs[0].get_header().size();
        std::cout << a<< std::endl;
        
        int index = 18;
        int i2 = 0;
        std::vector<std::string> values;
        for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
        {
            if (i2 == a)
                break;
            if (ptr->value().operator OpenXLSX::XLCellValue().typeAsString().compare("integer") == 0)
            {
                values.push_back(ft_itoa(ptr->value().operator OpenXLSX::XLCellValue()));
            }
            if (ptr->value().typeAsString().compare("empty") == 0)
            {
                values.push_back("");
            }
            if (ptr->value().operator OpenXLSX::XLCellValue().typeAsString().compare("string") == 0)
            {
                values.push_back(ptr->value().operator OpenXLSX::XLCellValue().get<std::string>());
            }
            
            //wkb.cell(XLCellReference(row, col)).value() = ptr->value().operator OpenXLSX::XLCellValue();
            col++;
            i2++;
        }
        std::cout << values.size() << std::endl;
        dbs[0].add_value(values);
        row++;
    }
    std::cout << dbs[0].get_size() << std::endl;
    show();
    write.save();
    write.close();
    /*for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
    {
        std::ofstream db_writee("values.txt", std::ios_base::app);
        auto type = ptr.operator*().value().type();
        db_writee << ptr->value().operator OpenXLSX::XLCellValue() << std::endl;
    }*/
}