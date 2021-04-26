# en.UTF-8

This is a fork from citra ( https://github.com/citra-emu/citra ), the 3DS emulator. I was not trying something revolutionary, but the main tasks are incomplete: "Reset()" function is broken thus the reset shortcut is left "unimplemented" (F12). The "RequestShutdown()" however works without any problems, so you can quit the emulator with the "ESC" key.

## ChangeLog cover...

Besides these the main routine for keyboard fetching was simplified, the "core/hle/kernel/ipc.h" is now "core/hle/server_ipc.h", the GDB stub is enabled with the definition "HAVE_GDBSTUB" (plus the object file in the Makefile), the "microprofile" and logging were completely nullified, the really bad hierarchy got a little breath, plus some small changes to deliver a "kickstart" on hacking great again!

## Dependencies.

Some external dependencies are inside the tree, just to worry less about them (xbyak is only about headers), but you still need to get and install some "separately": boost, cryptopp, dynarmic ( https://github.com/tvtoon/dynarmic ), and teakra. DO NOT USE BOOST 1.74, THAT COMES WITH GENTOO! There is a bug that crashes this emulator compilation, install the 1.75 or the one inside citra repository. CryptoPP should be done separately because of some processor specific stuff, the citra submodule was not a good idea. Dynarmic had a really bad problem: the "-O3" compilation switch bugged the hell out of the emulator, so it locked the threads or something like that, randomly crashing the emulator on startup, also my version comes without "fmt" dependency (used for debug and disassembly messages). Finally, teakra is only used in LLE audio "audio/lle.cpp", but since I had not changed a thing (hierarchy, Makefile), there isn't a need for repository.

Another really bad dependency to take note about is the libzstd, only used to compress and extract state files. I would rather disable it completely, plus saving state, but since I don't want to change more things and it is useful for debugging (for a while), have at you.

## Macros.

The good thing about citra is the lack of this trash inside the source, so there are only few one relevant.

* Many files:#ifdef ENABLE_WEB_SERVICE
* common/common_paths.h: USER_DIR.
* video_core/shader/shader.cpp: ARCHITECTURE_x86_64.
* video_core/renderer_opengl/gl_rasterizer_cache.h: __GNUC__.
* video_core/renderer_opengl/gl_shader_util.h: GL_FRAGMENT_PRECISION_HIGH.
* For FFMPEG decoding (recording video not implemented in CLI): HAVE_FFMPEG.
* The same for Windows, audio_core/hle/hle.cpp: HAVE_MF.

## FMT.

It was not removed because of some massive use inside "video_core/renderer_opengl/*", that needs a very well thought rewrite.

Just for curiosity, these are the files that need to be changed...
* audio_core/sdl2_sink.cpp~
* audio_core/sink_details.cpp~
* citra/citra.cpp~
* citra/emu_window_sdl2.cpp~
* common/logging/backend.cpp~
* common/logging/log.h~
* common/logging/text_formatter.cpp~
* common/microprofile.h~
* common/thread.cpp~
* common/timer.cpp~
* common/timer.h~
* common/x64/xbyak_abi.h~
* common/x64/xbyak_util.h~
* core/arm/dynarmic/arm_dynarmic.cpp~
* core/arm/dyncom/arm_dyncom_interpreter.cpp~
* core/cheats/cheats.cpp~
* core/core.cpp~
* core/custom_tex_cache.cpp~
* core/file_sys/archive_extsavedata.cpp~
* core/file_sys/archive_extsavedata.h~
* core/file_sys/archive_source_sd_savedata.cpp~
* core/file_sys/archive_systemsavedata.cpp~
* core/file_sys/cia_container.cpp~
* core/file_sys/ncch_container.cpp~
* core/file_sys/seed_db.cpp~
* core/gdbstub/gdbstub.cpp~
* core/hle/kernel/svc.cpp~
* core/hle/service/am/am.cpp~
* core/hle/service/cecd/cecd.cpp~
* core/hle/service/cfg/cfg.cpp~
* core/hle/service/fs/archive.cpp~
* core/hle/service/gsp/gsp_gpu.cpp~
* core/hle/service/nwm/uds_connection.cpp~
* core/hle/service/service.cpp~
* core/hw/aes/key.cpp~
* core/hw/gpu.cpp~
* core/hw/rsa/rsa.cpp~
* core/loader/ncch.cpp~
* core/movie.cpp~
* core/perf_stats.cpp~
* core/savestate.cpp~
* input_common/main.cpp~
* input_common/sdl/sdl.cpp~
* input_common/sdl/sdl_impl.cpp~
* input_common/touch_from_button.cpp~
* network/network.cpp~
* network/room.cpp~
* video_core/command_processor.cpp~
* video_core/renderer_opengl/gl_rasterizer_cache.cpp~
* video_core/renderer_opengl/gl_rasterizer.cpp~
* video_core/renderer_opengl/gl_resource_manager.cpp~
* video_core/renderer_opengl/gl_shader_decompiler.cpp~
* video_core/renderer_opengl/gl_shader_gen.cpp~
* video_core/renderer_opengl/gl_stream_buffer.cpp~
* video_core/renderer_opengl/renderer_opengl.cpp~
* video_core/renderer_opengl/texture_filters/anime4k/anime4k_ultrafast.cpp~
* video_core/renderer_opengl/texture_filters/bicubic/bicubic.cpp~
* video_core/renderer_opengl/texture_filters/scale_force/scale_force.cpp~
* video_core/renderer_opengl/texture_filters/xbrz/xbrz_freescale.cpp~
* video_core/shader/shader.cpp~
* video_core/shader/shader_interpreter.cpp~
* video_core/shader/shader_jit_x64_compiler.h~
* video_core/shader/shader_jit_x64.cpp~
* video_core/swrasterizer/rasterizer.cpp~
