#pragma once

#include <stdexcept>
#include "Core/Array2D.h"

namespace ARRAY_2D_TESTS
{
    TEST_CASE("Array dimensions properly set upon construction.", "[Array2D]")
    {
        // CREATE A 2D ARRAY.
        const unsigned int WIDTH = 4;
        const unsigned int HEIGHT = 3;
        CORE::Array2D<int> array_2d(WIDTH, HEIGHT);

        // VALIDATE THE DIMENSIONS.
        unsigned int actualWidth = array_2d.GetWidth();
        REQUIRE(WIDTH == actualWidth);

        unsigned int actualHeight = array_2d.GetHeight();
        REQUIRE(HEIGHT == actualHeight);
    }

    TEST_CASE("An element's value can be modified.", "[Array2D]")
    {
        // CREATE A 2D ARRAY.
        const unsigned int WIDTH = 4;
        const unsigned int HEIGHT = 3;
        CORE::Array2D<int> array_2d(WIDTH, HEIGHT);

        // SET AN ELEMENT.
        const int ELEMENT_X = 2;
        const int ELEMENT_Y = 1;
        const int ELEMENT_VALUE = 72;
        array_2d(ELEMENT_X, ELEMENT_Y) = ELEMENT_VALUE;

        // VALIDATE THAT THE ELEMENT WAS PROPERLY SET.
        int actualElement = array_2d(ELEMENT_X, ELEMENT_Y);
        REQUIRE(ELEMENT_VALUE == actualElement);
    }

    TEST_CASE("Minimum 2D coordinates are valid.", "[Array2D]")
    {
        // CREATE A 2D ARRAY.
        const unsigned int WIDTH = 4;
        const unsigned int HEIGHT = 3;
        CORE::Array2D<int> array_2d(WIDTH, HEIGHT);

        // SET THE ELEMENT AT THE MINIMUM COORDINATES.
        const unsigned int MIN_X = 0;
        const unsigned int MIN_Y = 0;
        const int ELEMENT_VALUE = 72;
        array_2d(MIN_X, MIN_Y) = ELEMENT_VALUE;

        // VALIDATE THE ELEMENT.
        int actualElement = array_2d(MIN_X, MIN_Y);
        REQUIRE(ELEMENT_VALUE == actualElement);
    }

    TEST_CASE("Maximum 2D coordinates are valid.", "[Array2D]")
    {
        // CREATE A 2D ARRAY.
        const unsigned int WIDTH = 4;
        const unsigned int HEIGHT = 3;
        CORE::Array2D<int> array_2d(WIDTH, HEIGHT);

        // SET THE ELEMENT AT THE MAXIMUM COORDINATES.
        const unsigned int MAX_X = WIDTH - 1;
        const unsigned int MAX_Y = HEIGHT - 1;
        const int ELEMENT_VALUE = 72;
        array_2d(MAX_X, MAX_Y) = ELEMENT_VALUE;

        // VALIDATE THE ELEMENT.
        int actualElement = array_2d(MAX_X, MAX_Y);
        REQUIRE(ELEMENT_VALUE == actualElement);
    }

    TEST_CASE("Coordinates at the beginning of a row are valid.", "[Array2D]")
    {
        // CREATE A 2D ARRAY.
        const unsigned int WIDTH = 4;
        const unsigned int HEIGHT = 3;
        CORE::Array2D<int> array_2d(WIDTH, HEIGHT);

        // SET THE ELEMENT AT THE BEGINNING OF THE SECOND ROW.
        const unsigned int MIN_X = 0;
        const unsigned int ELEMENT_Y = 1;
        const int ELEMENT_VALUE = 72;
        array_2d(MIN_X, ELEMENT_Y) = ELEMENT_VALUE;

        // VALIDATE THE ELEMENT.
        int actualElement = array_2d(MIN_X, ELEMENT_Y);
        REQUIRE(ELEMENT_VALUE == actualElement);
    }

    TEST_CASE("Coordinates at the end of a row are valid.", "[Array2D]")
    {
        // CREATE A 2D ARRAY.
        const unsigned int WIDTH = 4;
        const unsigned int HEIGHT = 3;
        CORE::Array2D<int> array_2d(WIDTH, HEIGHT);

        // SET THE ELEMENT AT THE END OF THE FIRST ROW.
        const unsigned int MAX_X = WIDTH - 1;
        const unsigned int ELEMENT_Y = 0;
        const int ELEMENT_VALUE = 72;
        array_2d(MAX_X, ELEMENT_Y) = ELEMENT_VALUE;

        // VALIDATE THE ELEMENT.
        int actualElement = array_2d(MAX_X, ELEMENT_Y);
        REQUIRE(ELEMENT_VALUE == actualElement);
    }

    TEST_CASE("An x-coordinate outside the array bounds results in an exception.", "[Array2D]")
    {
        // CREATE A 2D ARRAY.
        const unsigned int WIDTH = 4;
        const unsigned int HEIGHT = 3;
        CORE::Array2D<int> array_2d(WIDTH, HEIGHT);

        // TRY ACCESSING AN ELEMENT OUTSIDE OF THE ARRAY'S HORIZONTAL BOUNDS.
        bool exceptionThrown = false;
        try
        {
            const unsigned int ELEMENT_X = WIDTH;
            const unsigned int ELEMENT_Y = 0;
            array_2d(ELEMENT_X, ELEMENT_Y);
        }
        catch (const std::out_of_range&)
        {
            exceptionThrown = true;
        }

        // VALIDATE THAT AN EXCEPTION WAS THROWN.
        REQUIRE(exceptionThrown);
    }

    TEST_CASE("A y-coordinate outside the array bounds results in an exception.", "[Array2D]")
    {
        // CREATE A 2D ARRAY.
        const unsigned int WIDTH = 4;
        const unsigned int HEIGHT = 3;
        CORE::Array2D<int> array_2d(WIDTH, HEIGHT);

        // TRY ACCESSING AN ELEMENT OUTSIDE OF THE ARRAY'S VERTICAL BOUNDS.
        bool exceptionThrown = false;
        try
        {
            const unsigned int ELEMENT_X = 0;
            const unsigned int ELEMENT_Y = HEIGHT;
            array_2d(ELEMENT_X, ELEMENT_Y);
        }
        catch (const std::out_of_range&)
        {
            exceptionThrown = true;
        }

        // VALIDATE THAT AN EXCEPTION WAS THROWN.
        REQUIRE(exceptionThrown);
    }
}