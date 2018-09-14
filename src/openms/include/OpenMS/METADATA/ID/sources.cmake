### the directory name
set(directory include/OpenMS/METADATA/ID)

### list all header files of the directory here
set(sources_list_h
DataQuery.h
IdentificationData.h
IdentificationDataConverter.h
IdentifiedMolecule.h
MetaData.h
ParentMolecule.h
)

### add path to the filenames
set(sources_h)
foreach(i ${sources_list_h})
  list(APPEND sources_h ${directory}/${i})
endforeach(i)

### source group definition
source_group("Header Files\\OpenMS\\METADATA\\ID" FILES ${sources_h})

set(OpenMS_sources_h ${OpenMS_sources_h} ${sources_h})

