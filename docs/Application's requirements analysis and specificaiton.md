# Application's requirements analysis and specification
It is required to create a first person sandbox multiplayer game based on cubes

## Requirements:
- **Account creation**\
The player will need to create an account by just entering a name when joining a server. The account stuff will become serious once the game has covered all the fundamental requirements
- **Multiplayer support**\
Multiplayer support will be one of the main game's aspects, so it needs to be dealt with as soon as possible.\
To implement it, it will be required to create a separate application called server.\
The server purpose is to:
	- welcome every player, unless they are in a ban list
	- implement new possible commands to authenticate the player
	- create the world and send it to every player

	The player that hosts the server will automatically have the admin role
- **Cube based world**\
The world will be made out of cubes and it will be editable. That means that a player will be able to place and remove any cube.\
For starter, there will be only 5 types of cubes: grass, dirt, stone, wood and leaves
- **Infinite procedural terrain generation**\
The world will be infinite and randomly generated in all axes. There will be mountains, caves and planes and trees all over the surface
- **Save world data**\
Players can save the world whenever they want to do so. It is also required to implement an autosave feature every 5 minutes
- **GUI support**\
Basic GUI support to pause the game, change settings, quit the game and select cube types from a menu.\
Basic debug info visualization (FPS counter, ping, coordinates, etc...)
- **Textual chat**\
Chat used to input commands or to chat with other players during a multiplayer session.\
The commands will be:
	- `/help` to get a list of all commands
	- `/ping` to calculate the server ping in milliseconds
	- `/dupeip [PlayerID]` to check all usernames linked to the same IP address (the banned users are marked in red, offline users in gray and online users in green)
	- `/mutechat [PlayerID] {<timeout> | inf}` to ban the player from the textual chat
	- `/mutevc [PlayerID] {<timeout> | inf}` to ban the player from the voice chat
	- `/mute [PlayerID] {<timeout> | inf}` to ban the player from textual and voice chat
	- `/kick [PlayerID] {message}` to kick a player from a server
	- `/ban [PlayerID] {message}` to ban a player from a server
	- `/ipban [PlayerID] {message}` to ban the player's ip and id from a server
	- `/unban [PlayerID]` to remove the player's ban from a server
	- `/op [PlayerID]` to give admin role to a player
- **Proximity voice chat**\
Voice chat to communicate with other players during a multiplayer session. The audio has to be 3D
- **Mod & Plugin support**\
Mods to edit world creation and plugins for servers to add new commands