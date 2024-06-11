# Page directory at block 00000
page_directory = {
# Index : [Writable bit, Physical block number]
'080' : ['0', '00001'], # Points to a page table for the range 0080C000 to 0080EFFF (read-only)
'100' : ['1', '00002'], # Points to a page table for the range 0100A000 to 0100CFFF (read/write)
}


# Page table for the first memory region (read-only), starting at block 00001
page_table_1 = {
# Index : [Writable bit, Physical block number]
'00C' : ['0', '00003'], # Maps 0080C000 to a physical block (read-only)
'00D' : ['0', '00004'], # Maps 0080D000 to the next physical block (read-only)
'00E' : ['0', '00005'], # Maps 0080E000 to the next physical block (read-only)
}


# Page table for the second memory region (read/write), starting at block 00002
page_table_2 = {
# Index : [Writable bit, Physical block number]
'00A' : ['1', '00006'], # Maps 0100A000 to a physical block (read/write)
'00B' : ['1', '00007'], # Maps 0100B000 to the next physical block (read/write)
'00C' : ['1', '00008'], # Maps 0100C000 to the next physical block (read/write)
}


# Outputting the structures for clarity
print("Page Directory (at block 00000):")
for index, entry in page_directory.items():
print(f"Index: {index}, Writable: {entry[0]}, Points to Page Table Block: {entry[1]}")


print("\nPage Table 1 (read-only, at block 00001):")
for index, entry in page_table_1.items():
print(f"Index: {index}, Writable: {entry[0]}, Points to Memory Block: {entry[1]}")


print("\nPage Table 2 (read/write, at block 00002):")
for index, entry in page_table_2.items():
print(f"Index: {index}, Writable: {entry[0]}, Points to Memory Block: {entry[1]}")

