#include <OpenXLSX.hpp>
#include <vector>
//#include <external/nowide/nowide/iostream.hpp>

using namespace OpenXLSX;
std::vector<XLCellValue> header;

int main() 
{

    XLDocument doc;
    doc.open("test.xlsx");
    auto wks = doc.workbook().worksheet("EQUIDAD");
    std::cout << wks.rowCount();
    std::ofstream db_writee("values.txt", std::ios_base::app);
    for (uint32_t i = 1; i < wks.rowCount(); i++)
    {
        auto rows = wks.row(i);
        auto cells = rows.cells();
        if (i == 1)
        {
            for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
            {
                header.push_back(ptr->value().operator OpenXLSX::XLCellValue());
            }
            continue;
        }
        int index = 0;
        for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
        {
            std::string comp = "CANDIDATO";
            if (comp.compare(header[index].get<std::string>()) == 0)
            {
                db_writee << ptr->value().operator OpenXLSX::XLCellValue() << std::endl;
                break;
            }
        }
        
    }
    db_writee.close();

    /*for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
    {
        std::ofstream db_writee("values.txt", std::ios_base::app);
        auto type = ptr.operator*().value().type();
        db_writee << ptr->value().operator OpenXLSX::XLCellValue() << std::endl;
    }*/
}