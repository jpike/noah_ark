#pragma once

#include <SFML/System.hpp>

namespace GAMEPLAY
{
    /// The elapsed time for the flood.
    ///
    /// The timeline for the flood is as follows:
    /// - Noah's age = 600 - 2nd month, 17th day - Flood starts (Genesis 7:11)
    /// - 40 days/nights - Flood continues (Genesis 7:12, Genesis 7:17)
    /// - 150 days - Water prevails (Genesis 7:24, Genesis 8:3) - this leads up to month 7 below
    /// - 7th month, 17th day (5 months later) - Ark comes to rest on mountains of Ararat (Genesis 8:4)
    /// - 10th month, 1st day - Waters recede to point where mountaintops seen (Genesis 8:5) - + about 2 months (60 days) + about 13 days = about 223 days
    /// - +40 days - Noah sends raven + dove out (Genesis 8:6-7) - about 223 + 40 = 263 days (11th month, 11th day)
    /// - +7 days - Noah sends dove out again (Genesis 8:10) = 270 days (11th month, 18th day)
    /// - +7 days - Noah sends dove out again; it doesn't return (Genesis 8:12) = 277 days (11th month, 25th day)
    /// - 601st year of Noah's life - 1st month, 1st day - Noah removes covering of ark (Genesis 8:13)
    /// - 601st year of Noah's life - 2nd month, 27th day - Earth dried out, so exit from the ark (Genesis 8:14)
    ///
    /// That's effectively 1 year + 10 days.  Although exact number of "days" in a year could have varied back then,
    /// this game will assume 365 days in a year according to modern calendar.  That means 375 days total on the ark.
    /// @todo   Should we try and have shorter "months" or a time period to better match the following?
    /// - https://www.esv.org/resources/esv-global-study-bible/chart-01-03/
    /// - https://www.understandchristianity.com/timelines/chronology-flood/
    /// - https://answersingenesis.org/bible-timeline/biblical-overview-of-the-flood-timeline/
    /// - https://answersingenesis.org/bible-timeline/avian-flood-chronology-noah-window/
    ///
    /// That's too long for this game in terms of real gameplay time, so "game time" is sped up so that 1 day takes 3 minutes.
    /// "Game time" still results in a decent amount of real-world time in order to still communicate the feeling of the
    /// flood taking a long time.
    class FloodElapsedTime
    {
    public:
        // STATIC CONSTANTS.
        /// The number of days per month.
        /// 30 month day is simple approximation for this game, though it might be better to use varying days based on the Hebrew calendar.
        static constexpr unsigned int DAYS_PER_MONTH = 30;
        /// The day the flood is described as "prevailing" on the earth.
        /// 2nd month, 17th day - Flood starts (Genesis 7:11)
        /// +150 days (Genesis 7:24, Genesis 8:3)
        /// = 
        /// 7th month, 17th day (5 months later) - Ark comes to rest on mountains of Ararat (Genesis 8:4)
        static constexpr unsigned int DAY_COUNT_FOR_FLOODS_PREVAILING_ON_EARTH = 150;
        /// The number of months between the floods "prevailng" and starting to recede.
        /// 10th month, 1st day - Waters recede to point where mountaintops seen (Genesis 8:5)
        static constexpr unsigned int MONTH_COUNT_BETWEEN_FLOODS_PREVAILING_AND_RECEDING = 2;
        /// The number of days left in the 7th month for flood receding.
        /// 30 days per month - 17 days from 7th month = 13 remaining days in 7th month. (day 223)
        static constexpr unsigned int DAY_COUNT_IN_7TH_MONTH_FOR_FLOOD_RECEDING = 13;
        static constexpr unsigned int DAY_COUNT_FOR_FLOODS_PREVAILING_AND_RECEDING = (
            DAY_COUNT_FOR_FLOODS_PREVAILING_ON_EARTH + 
            DAY_COUNT_IN_7TH_MONTH_FOR_FLOOD_RECEDING +
            (DAYS_PER_MONTH * MONTH_COUNT_BETWEEN_FLOODS_PREVAILING_AND_RECEDING));
        /// The day count after initial flood receding until the first time a raven and dove are sent out.
        /// Genesis 8:6-8 (day 263).
        static constexpr unsigned int DAY_COUNT_AFTER_INITIAL_FLOOD_RECEDING_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING = 40;
        /// The total flood day cound until the first sending of a raven and dove.
        static constexpr unsigned int FLOOD_DAY_COUNT_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING = (
            DAY_COUNT_FOR_FLOODS_PREVAILING_AND_RECEDING + DAY_COUNT_AFTER_INITIAL_FLOOD_RECEDING_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING);
        /// The day count between the first and second sending of the dove.
        /// Genesis 8:10 (day 270).
        static constexpr unsigned int DAY_COUNT_BETWEEN_FIRST_AND_SECOND_DOVE_SENDINGS = 7;
        /// Thay day count until the second sending of the dove.
        static constexpr unsigned int FLOOD_DAY_COUNT_UNTIL_SECOND_DOVE_SENDING = FLOOD_DAY_COUNT_UNTIL_FIRST_RAVEN_AND_DOVE_SENDING + DAY_COUNT_BETWEEN_FIRST_AND_SECOND_DOVE_SENDINGS;
        /// The day count between the second and third sending of the dove.
        /// Genesis 8:12 (day 277).
        static constexpr unsigned int DAY_COUNT_BETWEEN_SECOND_AND_FINAL_DOVE_SENDINGS = 7;
        //// The total flood day count until the final sending of the dove.
        static constexpr unsigned int FLOOD_DAY_COUNT_UNTIL_FINAL_DOVE_SENDING = FLOOD_DAY_COUNT_UNTIL_SECOND_DOVE_SENDING + DAY_COUNT_BETWEEN_SECOND_AND_FINAL_DOVE_SENDINGS;
        /// The final day of the flood.
        static constexpr unsigned int FLOOD_FINAL_DAY_COUNT = 375;

        // STATIC METHODS.
        static void GetCurrentDayAndHour(
            const sf::Time& total_flood_elapsed_game_time,
            unsigned int& day_count,
            unsigned int& hour_of_current_day);
    };
}
