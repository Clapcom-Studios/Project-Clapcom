﻿using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace YmirEngine
{
    public class Particles
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void PlayEmitter(object go);
    }
}
