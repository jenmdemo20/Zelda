/*****************************************************************************
 * Copyright 2014 Christoph Wick
 *
 * This file is part of Zelda.
 *
 * Zelda is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Zelda is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Zelda. If not, see http://www.gnu.org/licenses/.
 *****************************************************************************/

#ifndef __StaticPluginLoader_H__
#define __StaticPluginLoader_H__

#include "Logger.hpp"
#include "Ogre.h"
#include "OgreConfigFile.h"

// Static plugin headers
#ifdef OGRE_STATIC_CgProgramManager
#  include "OgreCgPlugin.h"
#endif
#ifdef OGRE_STATIC_OctreeSceneManager
#  include "OgreOctreePlugin.h"
#endif
#ifdef OGRE_STATIC_ParticleFX
#  include "OgreParticleFXPlugin.h"
#endif
#ifdef OGRE_STATIC_BSPSceneManager
#  include "OgreBspSceneManagerPlugin.h"
#endif
#ifdef OGRE_STATIC_GL
#  include "OgreGLPlugin.h"
#endif
#ifdef OGRE_STATIC_GL3Plus
#  include "OgreGL3PlusPlugin.h"
#endif
#ifdef OGRE_STATIC_GLES
#  include "OgreGLESPlugin.h"
#endif
#ifdef OGRE_STATIC_GLES2
#  include "OgreGLES2Plugin.h"
#endif
#ifdef OGRE_STATIC_Direct3D9
#  include "OgreD3D9Plugin.h"
#endif
#ifdef OGRE_STATIC_Direct3D11
#  include "OgreD3D11Plugin.h"
#endif
#ifdef OGRE_STATIC_PCZSceneManager
#  include "OgrePCZPlugin.h"
#endif
#ifdef OGRE_STATIC_OctreeZone
#  include "OgreOctreeZonePlugin.h"
#endif

namespace Ogre
{
  /** Utility class for loading some plugins statically.
      @remarks
      When loading plugins statically, you are limited to loading plugins 
      that are known about at compile time. You should define preprocessor
      symbols depending on which plugins you want to load - the symbol being
      OGRE_STATIC_<pluginname>, with pluginname being the usual name of the
      plugin DLL (no file extension, no debug suffix, and without the Plugin_ 
      or RenderSystem_ prefix.)
  */
  class StaticPluginLoader
  {
  protected:
#ifdef OGRE_STATIC_CgProgramManager
    CgPlugin* mCgPlugin;
#endif
#ifdef OGRE_STATIC_OctreeSceneManager
    OctreePlugin* mOctreePlugin;
#endif
#ifdef OGRE_STATIC_ParticleFX
    ParticleFXPlugin* mParticleFXPlugin;
#endif
#ifdef OGRE_STATIC_BSPSceneManager
    BspSceneManagerPlugin* mBSPPlugin;
#endif
#ifdef OGRE_STATIC_PCZSceneManager
    PCZPlugin* mPCZPlugin;
#endif
#ifdef OGRE_STATIC_OctreeZone
    OctreeZonePlugin* mOctreeZonePlugin;
#endif
#ifdef OGRE_STATIC_GL
    GLPlugin* mGLPlugin;
#endif
#ifdef OGRE_STATIC_GL3Plus
    GL3PlusPlugin* mGL3PlusPlugin;
#endif
#ifdef OGRE_STATIC_GLES
    GLESPlugin* mGLESPlugin;
#endif
#ifdef OGRE_STATIC_GLES2
    GLES2Plugin* mGLES2Plugin;
#endif
#ifdef OGRE_STATIC_Direct3D9
    D3D9Plugin* mD3D9Plugin;
#endif
#ifdef OGRE_STATIC_Direct3D11
    D3D11Plugin* mD3D11Plugin;
#endif
  public:
    StaticPluginLoader() {}

    /** Load all the enabled plugins against the passed in root object. */
    void load()
    {
      LOGI("Loading static plugins...");
      Root& root  = Root::getSingleton();
#ifdef OGRE_STATIC_GL
      LOGI("  - static gl");
      mGLPlugin = OGRE_NEW GLPlugin();
      root.installPlugin(mGLPlugin);
#endif
#ifdef OGRE_STATIC_GL3Plus
      LOGI("  - static gl3plus");
      mGL3PlusPlugin = OGRE_NEW GL3PlusPlugin();
      root.installPlugin(mGL3PlusPlugin);
#endif
#ifdef OGRE_STATIC_GLES
      LOGI("  - static gles");
      mGLESPlugin = OGRE_NEW GLESPlugin();
      root.installPlugin(mGLESPlugin);
#endif
#ifdef OGRE_STATIC_GLES2
      LOGI("  - static gles2");
      mGLES2Plugin = OGRE_NEW GLES2Plugin();
      root.installPlugin(mGLES2Plugin);
#endif
#ifdef OGRE_STATIC_Direct3D9
      mD3D9Plugin = OGRE_NEW D3D9Plugin();
      root.installPlugin(mD3D9Plugin);
#endif

#ifdef OGRE_STATIC_Direct3D11
      mD3D11Plugin = OGRE_NEW D3D11Plugin();
      root.installPlugin(mD3D11Plugin);
#endif
#ifdef OGRE_STATIC_CgProgramManager
      mCgPlugin = OGRE_NEW CgPlugin();
      root.installPlugin(mCgPlugin);
#endif
#ifdef OGRE_STATIC_OctreeSceneManager
      mOctreePlugin = OGRE_NEW OctreePlugin();
      root.installPlugin(mOctreePlugin);
#endif
#ifdef OGRE_STATIC_ParticleFX
      mParticleFXPlugin = OGRE_NEW ParticleFXPlugin();
      root.installPlugin(mParticleFXPlugin);
#endif
#ifdef OGRE_STATIC_BSPSceneManager
      mBSPPlugin = OGRE_NEW BspSceneManagerPlugin();
      root.installPlugin(mBSPPlugin);
#endif
#ifdef OGRE_STATIC_PCZSceneManager
      mPCZPlugin = OGRE_NEW PCZPlugin();
      root.installPlugin(mPCZPlugin);
#endif
#ifdef OGRE_STATIC_OctreeZone
      mOctreeZonePlugin = OGRE_NEW OctreeZonePlugin();
      root.installPlugin(mOctreeZonePlugin);
#endif
      LOGI("   ... finished");
    }

    void unload()
    {
      // don't unload plugins, since Root will have done that. Destroy here.
#ifdef OGRE_STATIC_OctreeZone
      OGRE_DELETE mOctreeZonePlugin;
#endif
#ifdef OGRE_STATIC_PCZSceneManager
      OGRE_DELETE mPCZPlugin;
#endif
#ifdef OGRE_STATIC_BSPSceneManager
      OGRE_DELETE mBSPPlugin;
#endif
#ifdef OGRE_STATIC_ParticleFX
      OGRE_DELETE mParticleFXPlugin;
#endif
#ifdef OGRE_STATIC_OctreeSceneManager
      OGRE_DELETE mOctreePlugin;
#endif
#ifdef OGRE_STATIC_CgProgramManager
      OGRE_DELETE mCgPlugin;
#endif
#ifdef OGRE_STATIC_Direct3D9
      OGRE_DELETE mD3D9Plugin;
#endif
#ifdef OGRE_STATIC_Direct3D11
      OGRE_DELETE mD3D11Plugin;
#endif
#ifdef OGRE_STATIC_GL
      OGRE_DELETE mGLPlugin;
#endif
#ifdef OGRE_STATIC_GL3Plus
      OGRE_DELETE mGL3PlusPlugin;
#endif
#ifdef OGRE_STATIC_GLES
      OGRE_DELETE mGLESPlugin;
#endif
#ifdef OGRE_STATIC_GLES2
      OGRE_DELETE mGLES2Plugin;
#endif
            
    }

  };

}

#endif
