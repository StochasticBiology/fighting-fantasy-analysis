Fighting Fantasy combat analysis
====

* C [.c] for simulation and stochastic optimal control
* R [.R] for decision trees 
* Mathematica [.nb] for algebra and visualisation
* Bash [.sh] for wrapping the analysis

Requirements: _rpart_ library for R

All code should run in under ten minutes 

* run.sh : compiles and runs all simulation code (analysis then follows with Mathematica notebooks as below)

* previous-results.tar.gz -- previously-run results of simulation code 

* Fig 1 -- victory probability theory vs simulation for no luck
  * battlesim.c simulates combat dynamics with no Luck strategy -> simscan.txt
  * simscan-analyse.nb compares these simulation results with theoretical expression and visualises

* Fig 2 -- victory probability theory vs simulation for simple luck
  * simpleluck.c simulates combat dynamics with simple Luck strategy -> simpleluck.txt
  * simpleluck-analyse.nb compares these simulation results with theoretical expression and visualises

* Fig 3 and Fig S2, S4 -- optimal strategies
  * optimal-strategies.c runs stochastic optimal control approach to derive optimal strategies for each state -> optimal-strategies-[3/4].txt
  * optimal-strategies-analyse.nb visualises these results 

* Fig 4 and Fig S5 -- decision trees approximating optimal strategies
  * rtree.R produces these trees as PDFs which are then illustrated by hand

