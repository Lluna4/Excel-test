#include "db.hpp"
//TODO: Cambiar unordered_map a map


using namespace OpenXLSX;
std::vector<std::string> header;
//std::ofstream db_writee("log.txt", std::ios_base::app);
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

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

void load_to_db(XLWorksheet wks, int num)
{
    uint16_t col = 1;
    uint32_t row = 1;
    uint32_t count = wks.rowCount();

    std::cout << num << count << std::endl;
    for (uint32_t i = 1; i < 5000; i++)
    {
        if (i >= count)
            break;
        col = 1;
        auto rows = wks.row(i);
        auto cells = rows.cells();
        if (cells.begin()->value().typeAsString().compare("empty") == 0 && num == 1)
        {
            return;
        }
        if (i < 18 && num == 0)
            continue;
        int a = dbs[num].get_header().size();
        //std::cout << a<< std::endl;

        int index = 18;
        int i2 = 0;
        std::vector<std::string> values;
        for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
        {
            if (i2 == a)
                break;
            
            if (ptr == cells.begin() && num == 0)
                continue;
            std::string type = ptr->value().operator OpenXLSX::XLCellValue().typeAsString();
            switch (str2int(type.c_str()))
            {
            case str2int("integer"):
                values.push_back(ft_itoa(ptr->value().operator OpenXLSX::XLCellValue()));
                continue;
            
            case str2int("empty"):
                values.push_back(" ");
                continue;

            case str2int("string"):
                values.push_back(ptr->value().operator OpenXLSX::XLCellValue().get<std::string>());
                continue;

            default:
                values.push_back(" ");
            }
            
            //wkb.cell(XLCellReference(row, col)).value() = ptr->value().operator OpenXLSX::XLCellValue();
            col++;
            i2++;
        }
        int noempty = 0;
        for (int x = 0; x < dbs[num].header_size(); x++)
        {
            if (x >= values.size())
                break;
            if (values[x].empty() != true)
                noempty++;
        }
        if (noempty == 0)
            continue;

        if (values.size() == dbs[num].header_size())
        {
            dbs[num].add_value(values);
        }
        else
        {
            values.resize(dbs[num].header_size());
            dbs[num].add_value(values);
        }
        row++;
    }
}

/*void crear_usuarios()
{

}*/

int main() 
{

    XLDocument doc;
    XLDocument write;
    XLDocument comp;

    write.create("output.xlsx");
    doc.open("test.xlsx");
    comp.open("comp.xlsx");

    write.workbook().addWorksheet("EQUIDAD");
    auto wkb = write.workbook().worksheet("EQUIDAD");
    auto wks = doc.workbook().worksheet("Hoja1");
    auto wkc = comp.workbook().worksheet("EQUIDAD");

    std::cout << wks.rowCount() << std::endl;
    std::cout << wkc.rowCount() << std::endl;
    write.save();

    auto rows = wks.row(14);
    auto cells = rows.cells();
    //load header into db
    for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
    {
        if (ptr->value().operator OpenXLSX::XLCellValue().get<std::string>().compare("ITEM") == 0)
            std::cout << ptr->value().typeAsString() << ptr->value().operator OpenXLSX::XLCellValue() << std::endl;
        if (ptr->value().typeAsString().compare("empty") == 0)
            continue;
        header.push_back(ptr->value().operator OpenXLSX::XLCellValue().get<std::string>());
    }
    dbs.push_back(db("tabla", header));

    auto rows2 = wkc.row(1);
    auto cells2 = rows2.cells();
    std::vector<std::string> header2;
    //load header into db
    for (auto ptr = cells2.begin(); ptr != cells2.end(); ptr.operator++())
    {
        if (ptr->value().operator OpenXLSX::XLCellValue().get<std::string>().compare("ITEM") == 0)
            std::cout << ptr->value().typeAsString() << ptr->value().operator OpenXLSX::XLCellValue() << std::endl;
        if (ptr->value().typeAsString().compare("empty") == 0)
            continue;
        header2.push_back(ptr->value().operator OpenXLSX::XLCellValue().get<std::string>());
    }
    dbs.push_back(db("comp", header2));

    std::thread load_1(load_to_db, wks, 0);
    std::thread load_2(load_to_db, wkc, 1);

    load_1.join();
    load_2.join();
    
    
    std::cout << " ?" << dbs[0].get_size() << std::endl;
    std::cout << " ?" << dbs[1].get_size() << std::endl;
    //show();
    std::unordered_map<std::string, std::vector<int>> col = dbs[0].get_column("ITEM");
    std::unordered_map<std::string, std::vector<int>> col2 = dbs[1].get_column("CANDIDATO");
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>> querying = dbs[1].cache();

    /*
    for (const auto & [ key, value ] : col) 
    {
        std::cout << " value: " << key << " column: " << value[0] << ", row "  << value[1] << std::endl;
    }

    for (const auto & [ key, value ] : col2) 
    {
        std::cout << " column: " << key << " value :" << value[0] << ", "  << value[1] << std::endl;
    }*/

    //print to excel
    uint32_t row = 393;
    for (auto& [key, value]: col)
    {
        //std::cout << "A" << std::endl;
        if (col2.contains(key) != true)
        {
            if (key.empty() == true)
                continue;
            
            //std::vector<std::string> values = dbs[0].get_value(value[1]);
            int col = 0;
            for (uint16_t i = 0; i < dbs[0].header_size(); i++)
            {
                if (dbs[0].get_item(value[1], value[0]).compare(" ") == 0)
                    continue;
                wkc.cell(XLCellReference(row, col + 1)).value() = dbs[0].get_item(value[1], value[0]);
                break;
            }
            row++;
        }
    }

    write.save();
    write.close();
    doc.close();
    comp.save();
    comp.close();
}