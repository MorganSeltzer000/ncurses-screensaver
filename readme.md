ncurses-screensaver

The guide to the screensaver function

Options:
Giving any arguments into the program will take you into interactive mode, which asks you whether you want colored output. Otherwise the program will default to using colors, if the system supports it.

Math:
This function works by going through the values of x and y, and whenever x or y is greater than the maximum, it resets and goes back to the start. This is modeled mathematically by taking the modulo of the values of x and y if they were not reset ever. Normally, just adding 1 to both x and y infinitely would return back to the origin whenever lcm of x,y steps have been taken, as lcm%x==0 and lcm%y==0. This is problematic, as in order to color in the entire array, x*y steps must be taken. The only time this occurs is when x and y are relatively prime, the lcm can be found by multiplying the prime factors of x and y that are not shared in common. This is a rather rare case, and with the most common screen size being 20x80 (with lcm of a mere 80 out of the total 1600 coordinates), a solution had to be found.

x and y can be redefined as some coordinate space composed of the relatively prime factors of x and the relatively prime factors of y. For example, a 20x80 space would be reduced to a 1x4 space. This space must be able to be fully filled, as by definition these two numbers are relatively prime. The space may then be stretched by a common factor n of both x and y. In the example, the space could expand to (1*5)x(4*5). Thus there would be (n-1) empty spaces between any two filled spaces due to this stretch, as each coordinate is stretched by a factor of n, leading to the empty spaces between the old 0 and 1 being (0,n*1) noninclusive. This process may be repeated, leading to the empty spaces being in (0,n*m*...). In the example, this would be (0,4*5)=19 empty spaces in the first step. In this program, we fill these empty spaces by creating two composites of the common prime factors of x and y, and use that to decide when to step into an empty space and which one to step into. x determines which of the (n-1) spaces to increment by whenever the function would repeat a cycle of coordinates, and then whenever a number of steps equal to the second composite is taken we shift x again by 1. This corresponds to first shifting the coordinate space by the first composite, and then the second composite. The decision to use these two composites was made for visual effect, because otherwise one could simply take steps of only 1 until the entire empty space is filled, but that does not look as good.
