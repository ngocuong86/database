import sys
import leveldb
from openpyxl import Workbook

if len(sys.argv) < 3:
    print("Usage: python3 leveldb2excel.py <leveldb_path> <output_excel_file>")
    sys.exit(1)

leveldb_path = sys.argv[1]
output_excel_file = sys.argv[2]

# Open the LevelDB database
db = leveldb.LevelDB(leveldb_path)

# Create a new Excel workbook
workbook = Workbook()
worksheet = workbook.active

# Iterate over the LevelDB key-value pairs and populate the Excel worksheet
for key, value in db.RangeIter(include_value=True):
    # Extract the data from the LevelDB key-value pairs
    # and populate the respective cells in the Excel worksheet
    worksheet.append([key.decode('utf-8'), value.decode('utf-8')])

# Save the workbook as an Excel file
workbook.save(output_excel_file)
