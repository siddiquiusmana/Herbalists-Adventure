Program: Herbalists' Adventure

This program calculates how many plants a particular herbalist will be able to pick based
on their own personal wishes and the practical world restraints. This program utlizes 
Dynamic Memory Allocation to process information given to it through an in.txt.

The in.txt is formatted in the following manner:

------------------------------------------------
# plants
PlantName1 PlantType PlantAbundance
PlantName2 PlantType PlantAbundance
PlantName3 PlantType PlantAbundance
[...]
PlantName# PlantType PlantAbundance

# regions

RegionName1
# plants
PlantName1
PlantName2
[...]
PlantName#

[...]

RegionName#
# plants
PlantName1
PlantName2
[...]
PlantName#

# Herbalists		

Herbalist1
# Picks
# Regions
Region1
[...]
Region#

[...]

Herbalist#
# Picks
# Regions
Region1
[...]
Region#

----------------------------------------------------------

Where, PlantAbundance; # plants; # regions; # herbalists; # picks; are positive integers only.

This program then reads the input sequentially and stores the relevant information in 
dynamically allocated memory blocks.

It then calculates how many of each type of plants, each of the respective herbalists will be able
to pick based on:
-Regions they want to visit
-How many total plants they want to pick
-The abundance of that plant in that region

The program then prints out each of the Herbalists with their relevant visitations and the
number of plants they can pick in that region.

It, then, frees all the memory blocks to make sure no memory leaks occur, post-run.