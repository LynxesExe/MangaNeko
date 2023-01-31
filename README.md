# Manga Neko

## Was dis?

Plex but for Mangas, essentially.

The idea is having a server that can access (and add to) a collection of mangas, meaning CBZ, CBR, CB7 etc. It can then proceed to serve single pages or the entire thing.

This is a project I work on during my off time, meaning progress may be inconsistent, also, I'm really throwing it up with no intention of making it the *bestest, safestest* software ever, but rather, making it work, and expand from there.

Right now there isn't much here, I did some local test that I haven't pushed on GitHub since it was essentially... trash, but to explain what the basic idea is.

### What I'm planning

The idea is having a REST API service, essentially, based off of [Drogon](https://github.com/drogonframework/drogon), an awesome C++ web framework and bit7z, a library to use 7z (required to open the various archive formats, which by the way is great because it can open about every format you can think of... kinda).
For the first usable version, the goals are:

 - :white_check_mark: Being able to upload a large file from the APIs
 - :white_check_mark: Being able to extract single pages
 - :white_check_mark: Being able to redownlaod entire archives

Those were the main requirement to get this software up and running, I tested everything on a custom controller that I didn't push, except for the upload manga one.

I can confirm the ability to upload the archive and extract the pages individually, to then serve them.

### Stuff to do next

Setup a database, I'm considering wether it makes sense to support SQLite, MariaDB or both. A small DB will be required since keeping track of files location via their names in a directory isn't quite ideal for a variety of reasons.

Once that is done, the second part is deciding how to order the pages inside a manga, in case you didn't know CBZ and similar archives are created the following way... mostly (an actual standard doesn't exist, as far as I know):

 - ZIP Archive (or other format)
   - chapter 00
     - page 1
     - page 2
     - more pages
   - chapter 01
     - page 1
     - page 2

of course, nomenclature can change, but some std function to sort should work alright (or anyway, I saw some other projects doing it this way)

## Cool and all, but where is the frontend?

*ahem* there isn't one, for now. I mean come on, the pushed code can't even do much!

Quite frankly I'm not a frontend developer, so I doubt I could make a great frontend, however, there is a great app that I really enjoy using called [Tachiyomi](https://tachiyomi.org/), it's a manga reader for Android, but it also supports extensions (.apk files).

My plan for the moment is trying to make an extension for Tachityomi, it seems like they can be configured from the user once installed, to specify server address and port. This should allow us to use Tachiyomi as a frontend for MangaNeko, and it would be a great frontend too!

Of course, Tachiyomi is only for Android, the other idea is creating a WebApp frontend, a single page app made in Svelte (I'm considering), which considering how I'm making it, would be very basic, but serviceable.

I would also like, since I'm obsessed with it, a GTK4 cross platform frontend. Because I like GTK4 and don't like web apps _too_ much.

Anyway I'm not good at Kotlin or JavaScript, so, if someone knows either and believes in this thingy _\*(when there wil actually be something to believe in, that is)_, please do consider helping making a frontend or making a slick one on your own!

## That's it, this basic?

At first yeah, otherwise I'll end up writing a ton of boilerplate that will never end up doing anything.

In the future however, I would like to add a few things:

 - Users
 - Ownership of mangas by users
 - Sharing content between users
 - Creation of playlist and sharing of playlist
 - Bookmarks
 - Reviews (to sort out what you did and didn't like)

Most of those features will be incompatible with the eventual Tachiyomi extension, that will simply take whatever manga is available to you as a user and list that. Therefore, for now, I'm skipping on all of this.

## Why all this stuff?

I tried looking for something like this, a _"Plex"_ but for mangas, I didn't find one. So I thought, what the heck, let's try.

### Why C++?

Because that's what I like and know best.

### How to build?

Follow the drogon instructions and my CMake file will take care of the rest, for now it only works on Linux due to 7z. I'll be improving it afterwards since my development environment is Linux anyway, well, the WSL rather.

For now, I won't spend any other time writing this since there really isn't much. I wasn't planning on publishing this so far but I kinda had to. If you're interested tho, add a watch and nag me if I stop working on it.

Alright, I wasted enough time with a readme, but hey, now GitHub will stop asking! Check back later, bye!