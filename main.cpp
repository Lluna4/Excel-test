#include <OpenXLSX.hpp>
//#include <external/nowide/nowide/iostream.hpp>

using namespace OpenXLSX;

int main() 
{

    XLDocument doc;
    doc.open("test.xlsx");
    auto wks = doc.workbook().worksheet("FIABILIDAD");
    std::cout << wks.rowCount();
    auto rows = wks.row(4);
    auto cells = rows.cells();
    for (auto ptr = cells.begin(); ptr != cells.end(); ptr++)
    {
        std::cout << ptr.operator*().value() << std::endl;
    }
}