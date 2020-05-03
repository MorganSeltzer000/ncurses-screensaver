ncurses-screensaver

The guide to the screensaver function



Math:
This function works by going through the values of x and y, and whenever x or y is greater than the maximum, it resets and goes back to the start. This is modeled mathematically by taking the modulo of the values of x and y if they were not reset ever. Normally, just adding 1 to both x and y infinitely would return back to the origin whenever lcm of x,y steps have been taken, as lcm%x==0 and lcm%y==0. This is problematic, as in order to color in the entire array, x*y steps must be taken. The only time this occurs is when x and y are relatively prime, the lcm can be found by multiplying the prime factors of x and y that are not shared in common. This is a rather rare case, and with the most common screen size being 20x80 (with lcm of a mere 160 out of the total 1600 coordinates), a solution had to be found.
