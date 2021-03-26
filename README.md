# qtcards
Application for using [flashcards](https://en.wikipedia.org/wiki/Leitner_system)

# Dependencies
- Windows and Linux are both acceptable
- Qt 5.15.2+
- GCC 10.2.0+

# How to run
Simply proceed with the Qt Creator:
File->New File or Project->Import Project->Git clone->Repository: https://github.com/zRrrGet/qtcards.git

Or you can build it manually with qmake

# Usage
To start using application, it is needed to set up user profile and dictionary to study. Every dictionary can have access rights for specific profiles.
All It can be done by using options at the top-left corner.


Every pair of words in the dictionary have 6 stages. Each successful input of pair decrease stage up to zero. 
Pairs with high stages will appear frequently and vice versa(0 stage means word won't appear in the next sessions)
## Data storage format
Folder `dictionaries` contains files with *.dict* format. These files contains pair of words to be proceed in the program:
``` 
word1;word2;word1;word2;
```
Folder `profiles` contain *.conf* files.
```
[Accesses]
dictname=true

[English%20-%20Russian]
word1;word2=stageNum(0-6)

[Stats]
days=number(0-inf)
lastday=(serialized qvariant of date)
totalWordCount=number(0-inf)
```
