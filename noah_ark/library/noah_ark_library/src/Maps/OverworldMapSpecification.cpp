#include <sstream>
#include <stdexcept>
#include "Maps/OverworldMapSpecification.h"

using namespace MAPS;

OverworldMapSpecification::OverworldMapSpecification() :
    m_overworldWidthInTileMaps(0),
    m_overworldHeightInTileMaps(0),
    m_startingTileMapFilepath(),
    m_tileMapFilepaths()
{

}

OverworldMapSpecification::~OverworldMapSpecification()
{

}

bool OverworldMapSpecification::LoadFromFile(const std::string& filepath)
{
    // ATTEMPT TO LOAD THE XML DOCUMENT.
    TiXmlDocument xmlOverworldSpec(filepath.c_str());
    bool xmlFileLoadedSuccessfully = xmlOverworldSpec.LoadFile();
    if (!xmlFileLoadedSuccessfully)
    {
        return false;
    }

    // GET THE ROOT OF THE XML DOCUMENT.
    const std::string EXPECTED_ROOT_ELEMENT_NAME = "OverworldMap";
    TiXmlHandle xmlOverworldSpecDocumentHandle(&xmlOverworldSpec);
    TiXmlElement* pOverworldMapElement = xmlOverworldSpecDocumentHandle.FirstChild(EXPECTED_ROOT_ELEMENT_NAME.c_str()).Element();
    bool rootElementFound = (nullptr != pOverworldMapElement);
    if (!rootElementFound)
    {
        return false;
    }
    
    TiXmlHandle overworldMapElementHandle(pOverworldMapElement);
    
    // Values are stored in local variables before being copied to members
    // in order to make the operation of this method appear atomic.
    // If any part of the loading fails, we don't want the specification
    // to be left in a partial/bad state.

    // READ THE OVERWORLD'S WIDTH.
    const std::string EXPECTED_WIDTH_ELEMENT_NAME = "WidthInTileMaps";
    TiXmlElement* pOverworldWidthElement = overworldMapElementHandle.FirstChild(EXPECTED_WIDTH_ELEMENT_NAME.c_str()).Element();
    bool overworldWidthElementFound = (nullptr != pOverworldWidthElement);
    if (!overworldWidthElementFound)
    {
        return false;
    }

    // Convert the width from a string to an integer.
    unsigned int widthInTileMaps = 0;
    std::string widthString = pOverworldWidthElement->GetText();
    std::stringstream widthStringToIntConverter;
    widthStringToIntConverter << widthString;
    widthStringToIntConverter >> widthInTileMaps;
    bool widthValid = (widthInTileMaps > 0);
    if (!widthValid)
    {
        return false;
    }

    // READ THE OVERWORLD'S HEIGHT.
    const std::string EXPECTED_HEIGHT_ELEMENT_NAME = "HeightInTileMaps";
    TiXmlElement* pOverworldHeightElement = overworldMapElementHandle.FirstChild(EXPECTED_HEIGHT_ELEMENT_NAME.c_str()).Element();
    bool overworldHeightElementFound = (nullptr != pOverworldHeightElement);
    if (!overworldHeightElementFound)
    {
        return false;
    }

    // Convert the height from a string to an integer.
    unsigned int heightInTileMaps = 0;
    std::string heightString = pOverworldHeightElement->GetText();
    std::stringstream heightStringToIntConverter;
    heightStringToIntConverter << heightString;
    heightStringToIntConverter >> heightInTileMaps;
    bool heightValid = (heightInTileMaps > 0);
    if (!heightValid)
    {
        return false;
    }

    // READ IN THE STARTING MAP FILEPATH.
    const std::string EXPECTED_STARTING_TILE_MAP_FILEPATH_ELEMENT_NAME = "StartingTileMapFilepath";
    TiXmlElement* pStartingTileMapFilepathElement = overworldMapElementHandle.FirstChild(EXPECTED_STARTING_TILE_MAP_FILEPATH_ELEMENT_NAME.c_str()).Element();
    bool startingTileMapFilepathFound = (nullptr != pStartingTileMapFilepathElement);
    if (!startingTileMapFilepathFound)
    {
        return false;
    }

    std::string startingTileMapFilepath = pStartingTileMapFilepathElement->GetText();
    bool startingTileMapFilepathValid = !startingTileMapFilepath.empty();
    if (!startingTileMapFilepathValid)
    {
        return false;
    }

    // READ IN THE ALL OF THE TILE MAP FILEPATHS.
    // Keep track of the number of filepaths read so that we can properly calculate
    // the row/column position of each tile map and validate that the expected
    // number of maps were provided.
    unsigned int tileMapFilepathsReadCount = 0;
    std::vector< std::vector<std::string> > tileMapFilepaths;

    const std::string EXPECTED_TILE_MAPS_ELEMENT_NAME = "TileMaps";
    const std::string EXPECTED_TILE_MAP_FILEPATH_ELEMENT_NAME = "Filepath";
    TiXmlHandle firstTileMapFilepathElementHandle = overworldMapElementHandle
        .FirstChild(EXPECTED_TILE_MAPS_ELEMENT_NAME.c_str())
        .FirstChild(EXPECTED_TILE_MAP_FILEPATH_ELEMENT_NAME.c_str());
    for (TiXmlElement* pCurrentTileMapFilepathElement = firstTileMapFilepathElementHandle.Element();
        nullptr != pCurrentTileMapFilepathElement;
        pCurrentTileMapFilepathElement = pCurrentTileMapFilepathElement->NextSiblingElement())
    {
        // Read in the current tile map filepath.
        std::string currentTileMapFilepath = pCurrentTileMapFilepathElement->GetText();
        bool currentTileMapFilepathValid = !currentTileMapFilepath.empty();;
        if (!currentTileMapFilepathValid)
        {
            return false;
        }
        
        // Calculate the row/column position of the current tile map.
        unsigned int currentTileMapRow = tileMapFilepathsReadCount / heightInTileMaps;
        unsigned int currentTileMapColumn = tileMapFilepathsReadCount % widthInTileMaps;

        // Check if we need to create a new empty row.
        bool tileMapRowAlreadyExists = (currentTileMapColumn != 0);
        if (!tileMapRowAlreadyExists)
        {
            // Create an empty container to hold filepaths for this row's tile maps.
            tileMapFilepaths.push_back( std::vector<std::string>() );
        }

        // Save the current tile map filepath.
        tileMapFilepaths[currentTileMapRow].push_back(currentTileMapFilepath);
        ++tileMapFilepathsReadCount;
    }

    // VALIDATE THAT THE MAP WAS PROPERLY FORMED.
    // Verify the total number of filepaths.
    unsigned int expectedTileMapFilepathsCount = (widthInTileMaps * heightInTileMaps);
    bool expectedTileMapFilepathsRead = (expectedTileMapFilepathsCount == tileMapFilepathsReadCount);
    if (!expectedTileMapFilepathsRead)
    {
        return false;
    }

    // Verify the height of the overworld.
    bool expectedRowsExist = (tileMapFilepaths.size() == heightInTileMaps);
    if (!expectedRowsExist)
    {
        return false;
    }

    // Verify the width of each row.
    for (unsigned int tileMapRow = 0; tileMapRow < heightInTileMaps; ++tileMapRow)
    {
        bool expectedColumnsExist = (tileMapFilepaths[tileMapRow].size() == widthInTileMaps);
    }

    // STORE THE VALID SPECIFICATION IN THIS OBJECT.
    m_overworldWidthInTileMaps = widthInTileMaps;
    m_overworldHeightInTileMaps = heightInTileMaps;
    m_startingTileMapFilepath = startingTileMapFilepath;
    m_tileMapFilepaths = tileMapFilepaths;

    return true;
}

unsigned int OverworldMapSpecification::GetOverworldWidthInTileMaps() const
{
    return m_overworldWidthInTileMaps;
}

unsigned int OverworldMapSpecification::GetOverworldHeightInTileMaps() const
{
    return m_overworldHeightInTileMaps;
}

std::string OverworldMapSpecification::GetStartingTileMapFilepath() const
{
    return m_startingTileMapFilepath;
}

std::string OverworldMapSpecification::GetTileMapFilepath(const unsigned int row, const unsigned int column) const
{
    // VALIDATE THE ROW AND COLUMN COORDINATES.
    // We expect that the container containing the actual filepaths was previously validated
    // to have dimensions matching the separate height/width members of this object.
    bool rowNumberValid = (m_overworldHeightInTileMaps > row);
    if (!rowNumberValid)
    {
        std::stringstream invalidRowErrorMessage;
        invalidRowErrorMessage 
            << "Invalid row of " << row << " specified for a tile map filepath in an overworld.  "
            << "Expected a value less than " << m_overworldHeightInTileMaps << std::endl;
        throw std::out_of_range(invalidRowErrorMessage.str());
    }

    bool columnNumberValid = (m_overworldWidthInTileMaps > column);
    if (!columnNumberValid)
    {
        std::stringstream invalidColumnErrorMessage;
        invalidColumnErrorMessage 
            << "Invalid column of " << column << " specified for a tile map filepath in an overworld.  "
            << "Expected a value less than " << m_overworldWidthInTileMaps << std::endl;
        throw std::out_of_range(invalidColumnErrorMessage.str());
    }

    // GET THE REQUEST FILEPATH.
    // We expect that any empty values were not added in population of the member.
    const std::string& tileMapFilepath = m_tileMapFilepaths[row][column];
    return tileMapFilepath;
}