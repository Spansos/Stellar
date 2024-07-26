## Stellarium design document

1. [Planets](#1-planets)
  	1. [Organizations](#11-organizations)
  	2. [Demographics and species](#12-demographics-and-species)
  	3. [Political ideas and actions](#13-political-ideas-and-actions)
  	4. [Events & modifiers](#14-events--modifiers)
2. [Micro- & macrostructures](#2-micro---macrostructures)
3. [Military](#3-military)
    1. [Fleets](#31-fleets)
    2. [Planet and structure defenses](#32-planet-and-structure-defenses)
    3. [Ground offensives](#33-ground-offensives)
4. [States](#4-states)
    1. [de Jure control](#41-de-jure-control)
    2. [de Facto control](#42-de-facto-control)
    3. [Events & modifiers](#43-events--modifiers)


### 1. Planets

Planets have a couple of simulated aspects, namely:
1. [Organizations](#11-organizations)
2. [Demographics and species](#12-demographics-and-species)
3. [Political ideas and actions](#13-political-ideas-and-actions)
4. [Events & modifiers](#14-events--modifiers)

Organizations are where the populace are economically and politically active. They have members turning some products into some others.  
The populace is what acts within those organizations.  
One part of the demographics is political alignment. Political alignment may result in political organizations which have their own impacts on the planet and its populace.  
Events are things that may happen when their conditions are met, or at random. Events can result in effects, modifiers, on the planet, organization or demographic it targets.

#### 1.1. Organizations

Organizations themselves keep track of their organization type, number of members and their demographics, and any modifiers. Based on these factors, every organization makes goods, generates demands for goods, changes the planets demographics or changes modifiers of other organizations, organization types, its planet or its state.

Organization types are, for now, the following:
1. School
   educates population, consumes consumer goods. Requires some educated workers.
2. Research labs
   sometimes grants buffs and new options for other organizations, its planet, or its state. Requires educated workers.
3. Mine
   makes minerals
4. Farm
   makes food & organics
5. Foundry
   makes steel from minerals
6. Factory
   makes consumer goods from steel, minerals and organics
7. Aerospace company
   makes ships from steel. Requires educated workers.

Some of an organization's demands are hard and some are soft.
Soft demands are demands which are not strictly neccessary for its function, but would increase its efficiency or decrease negative outcomes. Tools and education demands are often soft.
Hard demands are demands absolutely necessary for its function, such as raw materials.
Demands can be fulfilled either by organizations on the planet itself, or by importing it from other planets. Importing things creates a demand for ships.

#### 1.2. Demographics and species

Every planet keeps track of their demographics, including age brackets, species, sexes, religions, political alignment, etc.
I have no clue how this would actually work. Im just saying shit here.

A population group exists out of a couple of axis:
   1. Age
   2. Species
   3. Job Sector
   4. Education
   5. Religion
   6. Political Leaning
   7. Class
Every day, all planets get updated. When they do, some number of births, deaths and migrations will be spread accross the population groups depending on their factors.
Then, all population groups compositions will be changed according to, essentially, a big, statistical state machine.

#### 1.3. Political ideas and actions

One part of the demographics is political alignment. A political population will result in political organizations. I am not going to exactly define how they're going to work, for i am but a clueless little guy.

#### 1.4. Events & modifiers

Somewhat self-explanatory.

### 2. Micro- & macrostructures

Construction sites can be made for galactic structures. These structures generate demands, much like planets, while being constructed. These demands must be fullfilled via imports, which progresses the construction.
Microstructures are small, mostly passive structures, about the size of a fleet or so. These are things like mining stations, outposts, infrastructure thingies, etc.
Macrostructures are larger, behaving more like planets with a populace and organizations.

### 3. Military

#### 3.1. Fleets

Fleets can be commisioned by state.

#### 3.2. Planet and structure defenses

Some organizations grant offensive or defensive capabilities.

#### 3.3. Ground offensives

Fleets contain personel, you can throw them at planets where battle is then initiated.

### 4. States

Every player controlls a state. This state has control/ownership over some set of planets, fleets, and structures. This control is divided into two catagories, de Facto and de Jure.

#### 4.1. de Jure control

De Jure ownership is control where you claim rightful ownership over something, regardless of whether that is respected. Anything you have de Jure control over, but not de Facto control is in effect not yours, but you do get casus beli for the (re-)conquest or policing of the territory. Things under de Jure control are also usually within you sphere of influence. They are more likely to align with you and go to you if they can't defend theirselves.

#### 4.2. de Facto control

De Facto control is yeah

#### 4.3. Events & modifiers

they are