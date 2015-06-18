Checking 2^31 numbers, 2 of which match:
* Single thread C implementation with background pthread logging:  **4m37.035s**
* OpenCL implementation scheduled across all available devices with background pthread logging:  **0m24.808s**

That's an **11.17x** acceleration!

Hardware:  MacBook Pro Late 2013
* Intel(R) Core(TM) i7-4960HQ CPU @ 2.60GHz (OpenCL 1.2)
* Iris Pro (OpenCL 1.2)
* GeForce GT 750M (OpenCL 1.2)

Inspired by [/r/ShowerThoughts](https://www.reddit.com/r/Showerthoughts/comments/3a3xzw/if_a1_b2_c3_etc_then_twohundredandfiftynine_259/).
