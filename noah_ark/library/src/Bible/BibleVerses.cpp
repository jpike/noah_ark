#include <vector>
#include "Bible/BibleVerses.h"

namespace BIBLE
{
    std::string ToString(const BibleBook book)
    {
        switch (book)
        {
            case BibleBook::INVALID:
                return "Invalid";
            case BibleBook::GENESIS:
                return "Genesis";
            case BibleBook::ISAIAH:
                return "Isaiah";
            case BibleBook::EZEKIEL:
                return "Ezekiel";
            case BibleBook::MATTHEW:
                return "Matthew";
            case BibleBook::LUKE:
                return "Luke";
            case BibleBook::HEBREWS:
                return "Hebrews";
            case BibleBook::FIRST_PETER:
                return "1 Peter";
            case BibleBook::SECOND_PETER:
                return "2 Peter";
            default:
                /// @todo   Error handling?
                return "Unknown";
        }
    }

    BibleVerse::BibleVerse(
        const BibleBook book,
        const unsigned int chapter,
        const unsigned int verse,
        const std::string& text) :
    Book(book),
    Chapter(chapter),
    Verse(verse),
    Text(text)
    {}

    bool BibleVerse::operator< (const BibleVerse& rhs) const
    {
        if (Book < rhs.Book) return true;
        if (Book > rhs.Book) return false;

        // The books are equal, so compare chapters.
        if (Chapter < rhs.Chapter) return true;
        if (Chapter > rhs.Chapter) return false;

        // The books and chapters are equal, so compare verse numbers.
        if (Verse < rhs.Verse) return true;

        // This verse is either equal to or greater than right-hand side verse.
        return false;
    }

    std::string BibleVerse::GetCitationString() const
    {
        std::string citation_string =
            BIBLE::ToString(Book) +
            " " +
            std::to_string(Chapter) +
            ":" +
            std::to_string(Verse);
        return citation_string;
    }

    std::string BibleVerse::ToString() const
    {
        std::string full_verse_string =
            Text +
            " - " +
            GetCitationString();
        return full_verse_string;
    }

    /// @todo   I'm not sure of the exact representation of these verses yet,
    /// so this might change later to be stored in some other way than a vector.
    /// The KJV version is used since it is in the public domain.
    const std::vector<BibleVerse> BIBLE_VERSES = 
    {
        BibleVerse(BibleBook::GENESIS, 5, 28, "And Lamech lived an hundred eighty and two years, and begat a son:"),
        BibleVerse(BibleBook::GENESIS, 5, 29, "And he called his name Noah, saying, This same shall comfort us concerning our work and toil of our hands, because of the ground which the Lord hath cursed."),
        BibleVerse(BibleBook::GENESIS, 5, 30, "And Lamech lived after he begat Noah five hundred ninety and five years, and begat sons and daughters:"),
        BibleVerse(BibleBook::GENESIS, 5, 31, "And all the days of Lamech were seven hundred seventy and seven years: and he died."),
        BibleVerse(BibleBook::GENESIS, 5, 32, "And Noah was five hundred years old: and Noah begat Shem, Ham, and Japheth."),
        BibleVerse(BibleBook::GENESIS, 6, 1, "And it came to pass, when men began to multiply on the face of the earth, and daughters were born unto them,"),
        BibleVerse(BibleBook::GENESIS, 6, 2, "That the sons of God saw the daughters of men that they were fair; and they took them wives of all which they chose."),
        BibleVerse(BibleBook::GENESIS, 6, 3, "And the Lord said, My spirit shall not always strive with man, for that he also is flesh: yet his days shall be an hundred and twenty years."),
        BibleVerse(BibleBook::GENESIS, 6, 4, "There were giants in the earth in those days; and also after that, when the sons of God came in unto the daughters of men, and they bare children to them, the same became mighty men which were of old, men of renown."),
        BibleVerse(BibleBook::GENESIS, 6, 5, "And God saw that the wickedness of man was great in the earth, and that every imagination of the thoughts of his heart was only evil continually."),
        BibleVerse(BibleBook::GENESIS, 6, 6, "And it repented the Lord that he had made man on the earth, and it grieved him at his heart."),
        BibleVerse(BibleBook::GENESIS, 6, 7, "And the Lord said, I will destroy man whom I have created from the face of the earth; both man, and beast, and the creeping thing, and the fowls of the air; for it repenteth me that I have made them."),
        BibleVerse(BibleBook::GENESIS, 6, 8, "But Noah found grace in the eyes of the Lord."),
        BibleVerse(BibleBook::GENESIS, 6, 9, "These are the generations of Noah: Noah was a just man and perfect in his generations, and Noah walked with God."),
        BibleVerse(BibleBook::GENESIS, 6, 10, "And Noah begat three sons, Shem, Ham, and Japheth."),
        BibleVerse(BibleBook::GENESIS, 6, 11, "The earth also was corrupt before God, and the earth was filled with violence."),
        BibleVerse(BibleBook::GENESIS, 6, 12, "And God looked upon the earth, and, behold, it was corrupt; for all flesh had corrupted his way upon the earth."),
        BibleVerse(BibleBook::GENESIS, 6, 13, "And God said unto Noah, The end of all flesh is come before me; for the earth is filled with violence through them; and, behold, I will destroy them with the earth."),
        BibleVerse(BibleBook::GENESIS, 6, 14, "Make thee an ark of gopher wood; rooms shalt thou make in the ark, and shalt pitch it within and without with pitch."),
        BibleVerse(BibleBook::GENESIS, 6, 15, "And this is the fashion which thou shalt make it of: The length of the ark shall be three hundred cubits, the breadth of it fifty cubits, and the height of it thirty cubits."),
        BibleVerse(BibleBook::GENESIS, 6, 16, "A window shalt thou make to the ark, and in a cubit shalt thou finish it above; and the door of the ark shalt thou set in the side thereof; with lower, second, and third stories shalt thou make it."),
        BibleVerse(BibleBook::GENESIS, 6, 17, "And, behold, I, even I, do bring a flood of waters upon the earth, to destroy all flesh, wherein is the breath of life, from under heaven; and every thing that is in the earth shall die."),
        BibleVerse(BibleBook::GENESIS, 6, 18, "But with thee will I establish my covenant; and thou shalt come into the ark, thou, and thy sons, and thy wife, and thy sons' wives with thee."),
        BibleVerse(BibleBook::GENESIS, 6, 19, "And of every living thing of all flesh, two of every sort shalt thou bring into the ark, to keep them alive with thee; they shall be male and female."),
        BibleVerse(BibleBook::GENESIS, 6, 20, "Of fowls after their kind, and of cattle after their kind, of every creeping thing of the earth after his kind, two of every sort shall come unto thee, to keep them alive."),
        BibleVerse(BibleBook::GENESIS, 6, 21, "And take thou unto thee of all food that is eaten, and thou shalt gather it to thee; and it shall be for food for thee, and for them."),
        BibleVerse(BibleBook::GENESIS, 6, 22, "Thus did Noah; according to all that God commanded him, so did he."),
        BibleVerse(BibleBook::GENESIS, 7, 1, "And the Lord said unto Noah, Come thou and all thy house into the ark; for thee have I seen righteous before me in this generation."),
        BibleVerse(BibleBook::GENESIS, 7, 2, "Of every clean beast thou shalt take to thee by sevens, the male and his female: and of beasts that are not clean by two, the male and his female."),
        BibleVerse(BibleBook::GENESIS, 7, 3, "Of fowls also of the air by sevens, the male and the female; to keep seed alive upon the face of all the earth."),
        BibleVerse(BibleBook::GENESIS, 7, 4, "For yet seven days, and I will cause it to rain upon the earth forty days and forty nights; and every living substance that I have made will I destroy from off the face of the earth."),
        BibleVerse(BibleBook::GENESIS, 7, 5, "And Noah did according unto all that the Lord commanded him."),
        BibleVerse(BibleBook::GENESIS, 7, 6, "And Noah was six hundred years old when the flood of waters was upon the earth."),
        BibleVerse(BibleBook::GENESIS, 7, 7, "And Noah went in, and his sons, and his wife, and his sons' wives with him, into the ark, because of the waters of the flood."),
        BibleVerse(BibleBook::GENESIS, 7, 8, "Of clean beasts, and of beasts that are not clean, and of fowls, and of every thing that creepeth upon the earth,"),
        BibleVerse(BibleBook::GENESIS, 7, 9, "There went in two and two unto Noah into the ark, the male and the female, as God had commanded Noah."),
        BibleVerse(BibleBook::GENESIS, 7, 10, "And it came to pass after seven days, that the waters of the flood were upon the earth."),
        BibleVerse(BibleBook::GENESIS, 7, 11, "In the six hundredth year of Noah's life, in the second month, the seventeenth day of the month, the same day were all the fountains of the great deep broken up, and the windows of heaven were opened."),
        BibleVerse(BibleBook::GENESIS, 7, 12, "And the rain was upon the earth forty days and forty nights."),
        BibleVerse(BibleBook::GENESIS, 7, 13, "In the selfsame day entered Noah, and Shem, and Ham, and Japheth, the sons of Noah, and Noah's wife, and the three wives of his sons with them, into the ark;"),
        BibleVerse(BibleBook::GENESIS, 7, 14, "They, and every beast after his kind, and all the cattle after their kind, and every creeping thing that creepeth upon the earth after his kind, and every fowl after his kind, every bird of every sort."),
        BibleVerse(BibleBook::GENESIS, 7, 15, "And they went in unto Noah into the ark, two and two of all flesh, wherein is the breath of life."),
        BibleVerse(BibleBook::GENESIS, 7, 16, "And they that went in, went in male and female of all flesh, as God had commanded him: and the Lord shut him in."),
        BibleVerse(BibleBook::GENESIS, 7, 17, "And the flood was forty days upon the earth; and the waters increased, and bare up the ark, and it was lift up above the earth."),
        BibleVerse(BibleBook::GENESIS, 7, 18, "And the waters prevailed, and were increased greatly upon the earth; and the ark went upon the face of the waters."),
        BibleVerse(BibleBook::GENESIS, 7, 19, "And the waters prevailed exceedingly upon the earth; and all the high hills, that were under the whole heaven, were covered."),
        BibleVerse(BibleBook::GENESIS, 7, 20, "Fifteen cubits upward did the waters prevail; and the mountains were covered."),
        BibleVerse(BibleBook::GENESIS, 7, 21, "And all flesh died that moved upon the earth, both of fowl, and of cattle, and of beast, and of every creeping thing that creepeth upon the earth, and every man:"),
        BibleVerse(BibleBook::GENESIS, 7, 22, "All in whose nostrils was the breath of life, of all that was in the dry land, died."),
        BibleVerse(BibleBook::GENESIS, 7, 23, "And every living substance was destroyed which was upon the face of the ground, both man, and cattle, and the creeping things, and the fowl of the heaven; and they were destroyed from the earth: and Noah only remained alive, and they that were with him in the ark."),
        BibleVerse(BibleBook::GENESIS, 7, 24, "And the waters prevailed upon the earth an hundred and fifty days."),
        BibleVerse(BibleBook::GENESIS, 8, 1, "And God remembered Noah, and every living thing, and all the cattle that was with him in the ark: and God made a wind to pass over the earth, and the waters assuaged;"),
        BibleVerse(BibleBook::GENESIS, 8, 2, "The fountains also of the deep and the windows of heaven were stopped, and the rain from heaven was restrained;"),
        BibleVerse(BibleBook::GENESIS, 8, 3, "And the waters returned from off the earth continually: and after the end of the hundred and fifty days the waters were abated."),
        BibleVerse(BibleBook::GENESIS, 8, 4, "And the ark rested in the seventh month, on the seventeenth day of the month, upon the mountains of Ararat."),
        BibleVerse(BibleBook::GENESIS, 8, 5, "And the waters decreased continually until the tenth month: in the tenth month, on the first day of the month, were the tops of the mountains seen."),
        BibleVerse(BibleBook::GENESIS, 8, 6, "And it came to pass at the end of forty days, that Noah opened the window of the ark which he had made:"),
        BibleVerse(BibleBook::GENESIS, 8, 7, "And he sent forth a raven, which went forth to and fro, until the waters were dried up from off the earth."),
        BibleVerse(BibleBook::GENESIS, 8, 8, "Also he sent forth a dove from him, to see if the waters were abated from off the face of the ground;"),
        BibleVerse(BibleBook::GENESIS, 8, 9, "But the dove found no rest for the sole of her foot, and she returned unto him into the ark, for the waters were on the face of the whole earth: then he put forth his hand, and took her, and pulled her in unto him into the ark."),
        BibleVerse(BibleBook::GENESIS, 8, 10, "And he stayed yet other seven days; and again he sent forth the dove out of the ark;"),
        BibleVerse(BibleBook::GENESIS, 8, 11, "And the dove came in to him in the evening; and, lo, in her mouth was an olive leaf pluckt off: so Noah knew that the waters were abated from off the earth."),
        BibleVerse(BibleBook::GENESIS, 8, 12, "And he stayed yet other seven days; and sent forth the dove; which returned not again unto him any more."),
        BibleVerse(BibleBook::GENESIS, 8, 13, "And it came to pass in the six hundredth and first year, in the first month, the first day of the month, the waters were dried up from off the earth: and Noah removed the covering of the ark, and looked, and, behold, the face of the ground was dry."),
        BibleVerse(BibleBook::GENESIS, 8, 14, "And in the second month, on the seven and twentieth day of the month, was the earth dried."),
        BibleVerse(BibleBook::GENESIS, 8, 15, "And God spake unto Noah, saying,"),
        BibleVerse(BibleBook::GENESIS, 8, 16, "Go forth of the ark, thou, and thy wife, and thy sons, and thy sons' wives with thee."),
        BibleVerse(BibleBook::GENESIS, 8, 17, "Bring forth with thee every living thing that is with thee, of all flesh, both of fowl, and of cattle, and of every creeping thing that creepeth upon the earth; that they may breed abundantly in the earth, and be fruitful, and multiply upon the earth."),
        BibleVerse(BibleBook::GENESIS, 8, 18, "And Noah went forth, and his sons, and his wife, and his sons' wives with him:"),
        BibleVerse(BibleBook::GENESIS, 8, 19, "Every beast, every creeping thing, and every fowl, and whatsoever creepeth upon the earth, after their kinds, went forth out of the ark."),
        BibleVerse(BibleBook::GENESIS, 8, 20, "And Noah builded an altar unto the Lord; and took of every clean beast, and of every clean fowl, and offered burnt offerings on the altar."),
        BibleVerse(BibleBook::GENESIS, 8, 21, "And the Lord smelled a sweet savour; and the Lord said in his heart, I will not again curse the ground any more for man's sake; for the imagination of man's heart is evil from his youth; neither will I again smite any more every thing living, as I have done."),
        BibleVerse(BibleBook::GENESIS, 8, 22, "While the earth remaineth, seedtime and harvest, and cold and heat, and summer and winter, and day and night shall not cease."),
        BibleVerse(BibleBook::GENESIS, 9, 1, "And God blessed Noah and his sons, and said unto them, Be fruitful, and multiply, and replenish the earth."),
        BibleVerse(BibleBook::GENESIS, 9, 2, "And the fear of you and the dread of you shall be upon every beast of the earth, and upon every fowl of the air, upon all that moveth upon the earth, and upon all the fishes of the sea; into your hand are they delivered."),
        BibleVerse(BibleBook::GENESIS, 9, 3, "Every moving thing that liveth shall be meat for you; even as the green herb have I given you all things."),
        BibleVerse(BibleBook::GENESIS, 9, 4, "But flesh with the life thereof, which is the blood thereof, shall ye not eat."),
        BibleVerse(BibleBook::GENESIS, 9, 5, "And surely your blood of your lives will I require; at the hand of every beast will I require it, and at the hand of man; at the hand of every man's brother will I require the life of man."),
        BibleVerse(BibleBook::GENESIS, 9, 6, "Whoso sheddeth man's blood, by man shall his blood be shed: for in the image of God made he man."),
        BibleVerse(BibleBook::GENESIS, 9, 7, "And you, be ye fruitful, and multiply; bring forth abundantly in the earth, and multiply therein."),
        BibleVerse(BibleBook::GENESIS, 9, 8, "And God spake unto Noah, and to his sons with him, saying,"),
        BibleVerse(BibleBook::GENESIS, 9, 9, "And I, behold, I establish my covenant with you, and with your seed after you;"),
        BibleVerse(BibleBook::GENESIS, 9, 10, "And I, behold, I establish my covenant with you, and with your seed after you;"),
        BibleVerse(BibleBook::GENESIS, 9, 11, "And I will establish my covenant with you, neither shall all flesh be cut off any more by the waters of a flood; neither shall there any more be a flood to destroy the earth."),
        BibleVerse(BibleBook::GENESIS, 9, 12, "And God said, This is the token of the covenant which I make between me and you and every living creature that is with you, for perpetual generations:"),
        BibleVerse(BibleBook::GENESIS, 9, 13, "I do set my bow in the cloud, and it shall be for a token of a covenant between me and the earth."),
        BibleVerse(BibleBook::GENESIS, 9, 14, "And it shall come to pass, when I bring a cloud over the earth, that the bow shall be seen in the cloud:"),
        BibleVerse(BibleBook::GENESIS, 9, 15, "And I will remember my covenant, which is between me and you and every living creature of all flesh; and the waters shall no more become a flood to destroy all flesh."),
        BibleVerse(BibleBook::GENESIS, 9, 16, "And the bow shall be in the cloud; and I will look upon it, that I may remember the everlasting covenant between God and every living creature of all flesh that is upon the earth."),
        BibleVerse(BibleBook::GENESIS, 9, 17, "And God said unto Noah, This is the token of the covenant, which I have established between me and all flesh that is upon the earth."),
        BibleVerse(BibleBook::GENESIS, 9, 18, "And the sons of Noah, that went forth of the ark, were Shem, and Ham, and Japheth: and Ham is the father of Canaan."),
        BibleVerse(BibleBook::GENESIS, 9, 19, "These are the three sons of Noah: and of them was the whole earth overspread."),
        /**
        @todo   Should these part of the main game?  Or are they outside the scope of the "ark" story?
        BibleVerse(BibleBook::GENESIS, 9, 20, "And Noah began to be an husbandman, and he planted a vineyard:"),
        BibleVerse(BibleBook::GENESIS, 9, 21, "And he drank of the wine, and was drunken; and he was uncovered within his tent."),
        BibleVerse(BibleBook::GENESIS, 9, 22, "And Ham, the father of Canaan, saw the nakedness of his father, and told his two brethren without."),
        BibleVerse(BibleBook::GENESIS, 9, 23, "And Shem and Japheth took a garment, and laid it upon both their shoulders, and went backward, and covered the nakedness of their father; and their faces were backward, and they saw not their father's nakedness."),
        BibleVerse(BibleBook::GENESIS, 9, 24, "And Noah awoke from his wine, and knew what his younger son had done unto him."),
        BibleVerse(BibleBook::GENESIS, 9, 25, "And he said, Cursed be Canaan; a servant of servants shall he be unto his brethren."),
        BibleVerse(BibleBook::GENESIS, 9, 26, "And he said, Blessed be the Lord God of Shem; and Canaan shall be his servant."),
        BibleVerse(BibleBook::GENESIS, 9, 27, "God shall enlarge Japheth, and he shall dwell in the tents of Shem; and Canaan shall be his servant."),
        BibleVerse(BibleBook::GENESIS, 9, 28, "And Noah lived after the flood three hundred and fifty years."),
        BibleVerse(BibleBook::GENESIS, 9, 29, "And all the days of Noah were nine hundred and fifty years: and he died."),*/
        BibleVerse(BibleBook::ISAIAH, 54, 9, "For this is as the waters of Noah unto me: for as I have sworn that the waters of Noah should no more go over the earth; so have I sworn that I would not be wroth with thee, nor rebuke thee."),
        BibleVerse(BibleBook::EZEKIEL, 14, 14, "Though these three men, Noah, Daniel, and Job, were in it, they should deliver but their own souls by their righteousness, saith the Lord God."),
        BibleVerse(BibleBook::EZEKIEL, 14, 20, "Though Noah, Daniel, and Job were in it, as I live, saith the Lord God, they shall deliver neither son nor daughter; they shall but deliver their own souls by their righteousness."),
        BibleVerse(BibleBook::MATTHEW, 24, 37, "But as the days of Noah were, so shall also the coming of the Son of man be."),
        BibleVerse(BibleBook::LUKE, 17, 27, "They did eat, they drank, they married wives, they were given in marriage, until the day that Noah entered into the ark, and the flood came, and destroyed them all."),
        BibleVerse(BibleBook::HEBREWS, 11, 7, "By faith Noah, being warned of God of things not seen as yet, moved with fear, prepared an ark to the saving of his house; by the which he condemned the world, and became heir of the righteousness which is by faith."),
        BibleVerse(BibleBook::FIRST_PETER, 3, 20, "Which sometime were disobedient, when once the longsuffering of God waited in the days of Noah, while the ark was a preparing, wherein few, that is, eight souls were saved by water."),
        BibleVerse(BibleBook::SECOND_PETER, 2, 5, "And spared not the old world, but saved Noah the eighth person, a preacher of righteousness, bringing in the flood upon the world of the ungodly;")
    };
}
