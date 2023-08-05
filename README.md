# libGD_PieChart_C : a C program using the GD library to draw pie chart

This program is coded in C and use the library [GD](https://libgd.github.io/).<br><br>
His purpose is to create pie chart.<br>
It has 3 layouts mode :<br>
- -n : _The standard pie chart layout_<br>
* -b : _The largest part of the pie is extended_<br>
+ -d : _Each part of the pie is extended depending on the value_<br>
The program `main.c` takes entry parameters in this form :<br>
`-mode string=value`<br>
<br>
Here are exemples :<br>
`./prog -n USA=40 France=30 China=22 Spain=20`<br>
`./prog -b USA=40 France=30 China=22 Spain=20`<br>
`./prog -d USA=40 France=30 China=22 Spain=20`<br>

You can find demos [here](https://github.com/valpaz/libGD_PieChart_C/blob/main/image_repository/) for possible output layout<br>

