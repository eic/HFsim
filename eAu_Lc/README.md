# BeAGLE Lc Production

The running cards and filtering macros for simulating Lc events in eAu
collisions can be found in this repository. 

1) Run BeAGLE simulation. Instructions can be found here:
https://eic.github.io/software/beagle.html

Current simulations use version BeAGLE_1.03.01

2) Convert output txt file into root file using 
```
root -l -b -q 'make_tree.C("eA.txt")'
```

3) Filter out Lc events
```
root -l -b -q 'filter_Lc.C("eA.root")'
```

For more details on the runing procedure, refer to "run_eAu.sh"
