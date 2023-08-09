#include <OpenXLSX.hpp>
//#include <external/nowide/nowide/iostream.hpp>

using namespace OpenXLSX;

int main() 
{

    XLDocument doc;
    doc.open("test.xlsx");
    auto wks = doc.workbook().worksheet("EQUIDAD");
    std::cout << wks.rowCount();
    auto rows = wks.row(4);
    auto cells = rows.cells();
    for (auto ptr = cells.begin(); ptr != cells.end(); ptr.operator++())
    {
        std::ofstream db_writee("values.txt", std::ios_base::app);
        auto type = ptr.operator*().value().type();
        db_writee << ptr->value().operator OpenXLSX::XLCellValue() << std::endl;
    }
}