#include "simple_logger.h"



#include "online.h"



    ////Online Stuff
    //Uint8 status = 0;
    ////Loop to choose if your client/host
    //while (1)
    //{
    //    if (gfc_input_command_down("host"))
    //    {
    //        slog("Clicked L");
    //        status = 1;
    //        break;
    //    }
    //    if (gfc_input_command_down("client"))
    //    {
    //        slog("Clicked K");
    //        status = -1;
    //        break;
    //    }
    //}
    //if (status == 1)
    //{
    //    SDLNet_Init();
    //    IPaddress ip;
    //    SDLNet_ResolveHost(&ip, NULL /*the server*/, 1234/*the port*/);

    //    //Create socket for TCP connection
    //    TCPsocket server = SDLNet_TCP_Open(&ip); //The server socket, get a connection attempt to server
    //    TCPsocket client; //Make an array or clients, or socket sets to make more clients
    //    const char* text = "HELLO CLIENT!\n";

    //    while (1)
    //    {

    //        slog("Waiting for a connection...");
    //        client = SDLNet_TCP_Accept(server); //if NULL, nothing tried to connect to it
    //        if (client)
    //        {
    //            //Here you can communicate with the client
    //            SDLNet_TCP_Send(client, text, strlen(text) + 1);
    //            SDLNet_TCP_Close(client);
    //            break;
    //        }
    //    }
    //    SDLNet_TCP_Close(server);

    //}
    //else
    //{
    //    SDLNet_Init();
    //    IPaddress ip;
    //    SDLNet_ResolveHost(&ip, "127.0.0.1" /*the server*/, 1234/*the port*/);

    //    //Create socket for TCP connection
    //    TCPsocket client = SDLNet_TCP_Open(&ip);
    //    char text[100];
    //    SDLNet_TCP_Recv(client, text, 100);
    //    slog(text);
    //    SDLNet_TCP_Close(client);
    //}




/*

@blakemartin4992
10 years ago
Until I have time to code it myself, all I can do is give you a road map of what to do.

1. Learn how to use SDL_net or something else to get data across a network. Here's a good page for that: http://content.gpwiki.org/SDL:Tutorial:Using_SDL_net

2. Create a Display class or something to hold the settings for creating a SDL_Window.

3. Define a protocol for conveying settings
across the network. ie. cmd: winheight: 600 etc. This and making the parser for it will be the hardest part. For starters I would just send one command at a time.

4.  During initialization, open a connection and wait for input. Once you have parsed enough commands to fill in the settings for the window, listen only for the command to show the window. Once, you get that command, close the connection and create the window.

Once you get that down, optimizations and ways to make your command system a lot more useful should become apparent to you.
Good Luck!
*/