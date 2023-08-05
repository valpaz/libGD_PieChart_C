# libGD_PieChart_C : a C program using the GD library to draw pie chart

This program is coded in C and use the library [GD](https://libgd.github.io/).<br>
His purpose is to create pie chart.<br>
It has 3 layouts mode :<br>
*-n : The standard pie chart layout<br>
*-b : The largest part of the pie is extended<br>
*-d : Each part of the pie is extended depending on the value<br>
<br>
The program `main.c` takes entry parameters in this form :<br>
`-mode string=value`<br>
Here are exemples :<br>
`main.c -n USA=40 France=30 China=22 Spain=20`<br>
`main.c -b USA=40 France=30 China=22 Spain=20`<br>
`main.c -d USA=40 France=30 China=22 Spain=20`<br>

You can find demos [here](https://github.com/valpaz/libGD_PieChart_C/blob/main/image_repository/) which are possible output<br>

