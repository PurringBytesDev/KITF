#include "MagixExternalDefinitions.h"

void MagixExternalDefinitions::initialize()
{
	loadUnitMeshes("UnitMeshes.cfg");
	loadUnitEmotes("UnitEmotes.cfg");
	loadItems((ENCRYPTED_ITEMS) ? "Items.dat" : "Items.cfg", ENCRYPTED_ITEMS);
	loadHotkeys("Hotkeys.cfg");
	attackList.clear();
	loadAttacks("ad1.dat", false);
	loadAttacks("CustomAttacks.cfg", true);
	critterList.clear();
	loadCritters("cd1.dat", false);
	loadCritters("CustomCritters.cfg", true);
	if (!XOR7FileGen("cd2.dat", "cd2.cfg", true, true))
		throw(Exception(9, "Corrupted Data File", "cd2.dat, please run the autopatcher."));
	else _unlink("cd2.cfg");
}
void MagixExternalDefinitions::initializeCapabilities(const RenderSystemCapabilities* capabilities)
{
	hasVertexProgram = capabilities->hasCapability(RSC_VERTEX_PROGRAM);
	hasFragmentProgram = capabilities->hasCapability(RSC_FRAGMENT_PROGRAM);
}
