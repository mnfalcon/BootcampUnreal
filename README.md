# BootcampUnreal
For Globant's Unreal Bootjam

Things that were done in C++ include:
- Player projectiles
- First/Third person camera switch
- Mana, stamina and their respective functionalities and regeneration
- Sprint
- "Dodge" (it's more like a blink)
- Lumber pickup functionality

Everything else was made using blueprints, a blackboard and a behaviour tree.

#######################################
Gameplay video: https://youtu.be/tbiaysgNo0g


#######################################
About the assets:

- Some textures were imported, some were made from scratch.
- All 3D models were either imported or are part of the Starter Content.
- Music and sound effects were taken from Soniss.com, Game Audio Bundle, that is distributed for free (as well as all the other assets I used).
I'll post the links to where I got each asset from afterwards.
- The animation for the sword attack, the firing, and death, were made by hand with the UE4 mannequin.

######################################
Controls:

Alt - Dodge
E - Fire
Left Mouse Button - Attack
Left Shit - Sprint
Q - Open Pause Menu
(I will add a widget or something with the controls)

#####################################
Things I'd like to improve:

Sword attacks mostly depend on the animation. I couldn't improve this feature yet because it was the last thing I included and spent most of the time fixing other things. But I am aware that the way in which I did it isn't probably the best way to do it, so I'll be working on that.

Both the firing and sword animation don't look pretty in third person mode.
The firing animation is performed pointing to where the character points, not the player (one way to fix this would be setting UseControllerRotationYaw to true in third person mode, and this option is true when in first person mode).

The "inventory" isn't really flexible but it could be improved. For now by default every item that is picked up increases de lumber quantity the player has, but if I wanted to create another item to pick up, I was thinking I could use tags, so if the actor has a tag like "isLumber" or something, only then will it increase the lumber quantity the player has. There's no functionality to drop yet, either.

Some of the code might not be in use and may be safe to delete, mainly due to things I tested that didn't work out and postponed the removal in case there were any conflicts afterwards.


