# NTP Client

This is a tiny NTP client for old UNIX, primarily aimed at A/UX, but will
probably work on anything with BSD sockets where the definitions in 
modern_types.h are valid.

It's based on David Lettier's "Let's make an NTP client in C", but massaged
to build under A/UX with gcc, and massaged to be closer to my own tastes
in maintainability (with some tidy-ups like multiple initialisations of
variables removed).

# Build

Under A/UX: tested with gcc-2.7.2 from jagubox (also available in my
aux-minivnc repository).

# Credits


_© 2014 David Lettier._, http://www.lettier.com/ (see 
[Let's make a NTP Client in C](https://lettier.github.io/posts/2016-04-26-lets-make-a-ntp-client-in-c.html)).

Portions © 2022, Rob Mitchelmore

## License

See [LICENSE](LICENSE).
