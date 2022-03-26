# Script based on https://tomeofpreach.wordpress.com/2013/06/22/makepak-py/ and modified/updated.
# Credit goes to The Tome of Preach (no license given for the Python snippet).

import os
import struct
import sys
 
# Class for collecting the file headers and metadata.
class FileEntry:
    filename = ""

    offset = 0
    length = 0
 
# Arguments are source directory, then target filename (for example, "pak1.pak").
source_directory = sys.argv[1]
output_pak_filename = sys.argv[2]
virtual_root_directory = sys.argv[3] # assets or locales.
 
output_pak_file = open(output_pak_filename, "wb")
 
# Write a dummy header wherewith to start.
output_pak_file.write(struct.Struct("<4s2l").pack(b"PACK", 0, 0))
 
# Walk the directory recursively, add the files, and record the file entries.
file_entries = []
offset = 12

for root_directory, sub_directories, files in os.walk(source_directory):
    for file in files:
        current_filepath = os.path.join(root_directory, file)

        current_file_entry = FileEntry()
        current_file_entry.filename = os.path.relpath(current_filepath, source_directory).replace("\\", "/")
        current_file_entry.filename = virtual_root_directory + "/" + current_file_entry.filename

        with open(current_filepath, "rb") as current_file:
            output_pak_file.write(current_file.read())

            current_file_entry.offset = offset
            current_file_entry.length = current_file.tell()

            offset += current_file_entry.length

        file_entries.append(current_file_entry)

table_size = 0
 
# After all the file data, write the list of entries.
for entry in file_entries:
    output_pak_file.write(struct.Struct("<56s").pack(entry.filename.encode("ascii")))
    output_pak_file.write(struct.Struct("<l").pack(entry.offset))
    output_pak_file.write(struct.Struct("<l").pack(entry.length))

    table_size += 64

# Return to the header and write the values correctly.
output_pak_file.seek(0)
output_pak_file.write(struct.Struct("<4s2l").pack(b"PACK", offset, table_size))
