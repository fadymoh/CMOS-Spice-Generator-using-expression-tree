This is a CMOS spice generator. It takes as an input an Infix boolean expression and can handle priority using brackets but not inversion
on the brackets themselves but inversion on the input (!a). We use an expression tree to handle the brackets, and then we traverse it in
the transistor scaling mode with argument passing to size each transistor and finally output the string containing the spice code 
of every transistor including the inverters. If you like the code give a star :)
