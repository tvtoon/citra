PROJECT = citra
MAJVER = 1.0
MINVER = 1
LIBS = ${PROJECT}
PROGS = ${PROJECT}

include make/conf
include make/cpplib
#include make/libpng.mk
#include make/zlib.mk

#Boost::serialization
#if (ARCHITECTURE_x86_64) target_link_libraries(common PRIVATE xbyak)
#cryptopp
# OpenSSL (?crypto?), lurlparser, json-headers, cpp-jwt, and httplib for WEBSERVICE!
# -DENABLE_WEB_SERVICE -DCPPHTTPLIB_OPENSSL_SUPPORT
#
ELIBFLAGS += `pkg-config --libs --static libavcodec libavformat libavutil libswscale libswresample libzstd sdl2`
#ELIBFLAGS += -lboost_serialization
ELIBFLAGS += -lcryptopp -ldynarmic -lteakra
#ELIBFLAGS += /usr/local/lib64/libcryptopp.a /usr/local/lib64/libdynarmic.a /usr/local/lib64/libteakra.a
# ENET ONLY
#CFLAGS = -DHAS_FCNTL=1 -DHAS_GETADDRINFO=1 -DHAS_GETHOSTBYADDR_R=1 -DHAS_GETHOSTBYNAME_R=1 -DHAS_GETNAMEINFO=1 -DHAS_INET_NTOP=1 -DHAS_INET_PTON=1 -DHAS_MSGHDR_FLAGS=1 -DHAS_POLL=1 -DHAS_SOCKLEN_T=1
CFLAGS = -DARCHITECTURE_x86_64 -DHAVE_FFMPEG -Iexternals -Iexternals/soundtouch -I. -O2 -Wall -Wextra -std=c++17 -o

DATA =
DOCS =
INFOS =
INCLUDES =
# EXTERNALS

## FMT
SRC = externals/fmt/format.cc externals/fmt/os.cc

## GLAD
SRC += externals/glad/glad.c

## INIH
SRC += externals/inih/ini.c externals/inih/INIReader.cpp

## NIHSTRO
SRC += externals/nihstro/common.cpp externals/nihstro/compare.cpp externals/nihstro/declaration.cpp externals/nihstro/floatop.cpp externals/nihstro/flowcontrol.cpp externals/nihstro/parser_assembly.cpp externals/nihstro/preprocessor.cpp

## SOUNDTOUCH
SRC += externals/soundtouch/AAFilter.cpp externals/soundtouch/BPMDetect.cpp externals/soundtouch/FIFOSampleBuffer.cpp externals/soundtouch/FIRFilter.cpp externals/soundtouch/InterpolateCubic.cpp externals/soundtouch/InterpolateLinear.cpp externals/soundtouch/InterpolateShannon.cpp externals/soundtouch/PeakFinder.cpp externals/soundtouch/RateTransposer.cpp externals/soundtouch/SoundTouch.cpp externals/soundtouch/TDStretch.cpp externals/soundtouch/cpu_detect_x86.cpp externals/soundtouch/mmx_optimized.cpp externals/soundtouch/sse_optimized.cpp

# AUDIO
SRC += audio/codec.cpp audio/dsp_interface.cpp audio/hle/adts_reader.cpp audio/hle/decoder.cpp audio/hle/filter.cpp audio/hle/hle.cpp audio/hle/mixers.cpp audio/hle/source.cpp audio/lle.cpp audio/interpolate.cpp audio/sink_details.cpp audio/time_stretch.cpp

SRC += audio/sdl2_sink.cpp
#audio/cubeb_sink.cpp audio/cubeb_input.cpp

#if(ENABLE_MF) SRC += audio/hle/wmf_decoder.cpp audio/hle/wmf_decoder_utils.cpp

#elseif(ENABLE_FFMPEG_AUDIO_DECODER)
SRC += audio/hle/ffmpeg_decoder.cpp audio/hle/ffmpeg_dl.cpp
# target_compile_definitions(audio_core PUBLIC HAVE_FFMPEG)

#elseif(ENABLE_FDK)
#SRC += audio/hle/fdk_decoder.cpp
#target_link_libraries(audio_core PRIVATE ${FDK_AAC})
#target_compile_definitions(audio_core PUBLIC HAVE_FDK)

## COMMON
#SRC += common/logging/backend.cpp common/logging/filter.cpp common/logging/text_formatter.cpp
SRC += common/detached_tasks.cpp common/cityhash.cpp common/file_util.cpp common/memory_ref.cpp common/microprofile.cpp common/misc.cpp common/param_package.cpp common/scm_rev.cpp common/string_util.cpp common/telemetry.cpp common/texture.cpp common/thread.cpp common/timer.cpp
SRC += common/zstd_compression.cpp
#if(ARCHITECTURE_x86_64)
SRC += common/x64/cpu_detect.cpp

## INPUT
SRC += input/analog_from_button.cpp input/keyboard.cpp input/main.cpp input/motion_emu.cpp input/touch_from_button.cpp
#SRC += input/client.cpp input/protocol.cpp input/udp.cpp
#if(SDL2_FOUND)
SRC += input/sdl.cpp input/sdl_impl.cpp

## CORE
#if (ARCHITECTURE_x86_64 OR ARCHITECTURE_ARM64)
SRC += core/arm_dynarmic.cpp core/arm_dynarmic_cp15.cpp
# core/arm/dynarmic/arm_dynarmic.h  core/arm/dynarmic/arm_dynarmic_cp15.h
#SRC += core/gdbstub.cpp

SRC += core/arm_dyncom.cpp core/arm_dyncom_dec.cpp core/arm_dyncom_interpreter.cpp core/arm_dyncom_thumb.cpp core/arm_dyncom_trans.cpp core/armstate.cpp core/armsupp.cpp core/vfp.cpp core/vfpdouble.cpp core/vfpinstr.cpp core/vfpsingle.cpp core/cheat_base.cpp core/cheats.cpp core/gateway_cheat.cpp core/core.cpp core/core_timing.cpp core/custom_tex_cache.cpp core/backend.cpp core/archive_backend.cpp core/archive_extsavedata.cpp core/archive_ncch.cpp core/archive_other_savedata.cpp core/archive_savedata.cpp core/archive_sdmc.cpp core/archive_sdmcwriteonly.cpp core/archive_selfncch.cpp core/archive_source_sd_savedata.cpp core/archive_systemsavedata.cpp core/cia_container.cpp core/disk_archive.cpp core/delay_generator.cpp core/ivfc_archive.cpp core/layered_fs.cpp core/ncch_container.cpp core/patch.cpp core/path_parser.cpp core/romfs_reader.cpp core/savedata_archive.cpp core/seed_db.cpp core/ticket.cpp core/title_metadata.cpp core/frontend/default_applets.cpp core/frontend/mii_selector.cpp core/frontend/swkbd.cpp core/frontend/blank_camera.cpp core/frontend/factory.cpp core/frontend/interface.cpp core/frontend/emu_window.cpp core/frontend/framebuffer_layout.cpp core/frontend/mic.cpp core/frontend/scope_acquire_context.cpp core/hle/applet.cpp core/hle/erreula.cpp core/hle/mii_selector.cpp core/hle/mint.cpp core/hle/swkbd.cpp core/hle/address_arbiter.cpp core/hle/client_port.cpp core/hle/client_session.cpp core/hle/config_mem.cpp core/hle/event.cpp core/hle/handle_table.cpp core/hle/hle_ipc.cpp core/hle/ipc.cpp core/hle/recorder.cpp core/hle/kernel.cpp core/hle/memory.cpp core/hle/mutex.cpp core/hle/object.cpp core/hle/process.cpp core/hle/resource_limit.cpp core/hle/semaphore.cpp core/hle/server_port.cpp core/hle/server_session.cpp core/hle/session.cpp core/hle/shared_memory.cpp core/hle/shared_page.cpp core/hle/svc.cpp core/hle/thread.cpp core/hle/timer.cpp core/hle/vm_manager.cpp core/hle/wait_object.cpp core/hle/lock.cpp core/hle/romfs.cpp

SRC += core/hle/service/ac.cpp core/hle/service/ac_i.cpp core/hle/service/ac_u.cpp core/hle/service/act.cpp core/hle/service/act_a.cpp core/hle/service/act_u.cpp core/hle/service/am.cpp core/hle/service/am_app.cpp core/hle/service/am_net.cpp core/hle/service/am_sys.cpp core/hle/service/am_u.cpp core/hle/service/applet_manager.cpp core/hle/service/apt.cpp core/hle/service/apt_a.cpp core/hle/service/apt_s.cpp core/hle/service/apt_u.cpp core/hle/service/ns.cpp core/hle/service/ns_s.cpp core/hle/service/bcfnt.cpp core/hle/service/boss.cpp core/hle/service/boss_p.cpp core/hle/service/boss_u.cpp core/hle/service/cam.cpp core/hle/service/cam_c.cpp core/hle/service/cam_q.cpp core/hle/service/cam_s.cpp core/hle/service/cam_u.cpp core/hle/service/cecd.cpp core/hle/service/cecd_ndm.cpp core/hle/service/cecd_s.cpp core/hle/service/cecd_u.cpp core/hle/service/cfg.cpp core/hle/service/cfg_i.cpp core/hle/service/cfg_nor.cpp core/hle/service/cfg_s.cpp core/hle/service/cfg_u.cpp core/hle/service/csnd_snd.cpp core/hle/service/dlp.cpp core/hle/service/dlp_clnt.cpp core/hle/service/dlp_fkcl.cpp core/hle/service/dlp_srvr.cpp core/hle/service/dsp_dsp.cpp core/hle/service/err_f.cpp core/hle/service/frd.cpp core/hle/service/frd_a.cpp core/hle/service/frd_u.cpp core/hle/service/archive.cpp core/hle/service/directory.cpp core/hle/service/file.cpp core/hle/service/fs_user.cpp core/hle/service/gsp.cpp core/hle/service/gsp_gpu.cpp core/hle/service/gsp_lcd.cpp core/hle/service/hid.cpp core/hle/service/hid_spvr.cpp core/hle/service/hid_user.cpp core/hle/service/http_c.cpp core/hle/service/extra_hid.cpp core/hle/service/ir.cpp core/hle/service/ir_rst.cpp core/hle/service/ir_u.cpp core/hle/service/ir_user.cpp core/hle/service/cro_helper.cpp core/hle/service/ldr_ro.cpp core/hle/service/mic_u.cpp core/hle/service/mvd.cpp core/hle/service/mvd_std.cpp core/hle/service/ndm_u.cpp core/hle/service/news.cpp core/hle/service/news_s.cpp core/hle/service/news_u.cpp core/hle/service/nfc.cpp core/hle/service/nfc_m.cpp core/hle/service/nfc_u.cpp core/hle/service/nim.cpp core/hle/service/nim_aoc.cpp core/hle/service/nim_s.cpp core/hle/service/nim_u.cpp core/hle/service/pm.cpp core/hle/service/pm_app.cpp core/hle/service/pm_dbg.cpp core/hle/service/ps_ps.cpp core/hle/service/ptm.cpp core/hle/service/ptm_gets.cpp core/hle/service/ptm_play.cpp core/hle/service/ptm_sets.cpp core/hle/service/ptm_sysm.cpp core/hle/service/ptm_u.cpp core/hle/service/dev.cpp core/hle/service/pxi.cpp core/hle/service/qtm.cpp core/hle/service/qtm_c.cpp core/hle/service/qtm_s.cpp core/hle/service/qtm_sp.cpp core/hle/service/qtm_u.cpp core/hle/service/service.cpp core/hle/service/sm.cpp core/hle/service/srv.cpp core/hle/service/soc_u.cpp core/hle/service/ssl_c.cpp core/hle/service/y2r_u.cpp

SRC += core/arithmetic128.cpp core/ccm.cpp core/key.cpp core/gpu.cpp core/hw.cpp core/lcd.cpp core/rsa.cpp core/y2r.cpp core/3dsx.cpp core/elf.cpp core/loader.cpp core/ncch.cpp core/smdh.cpp core/memory.cpp core/movie.cpp core/perf_stats.cpp core/settings.cpp core/telemetry_session.cpp core/recorder.cpp
# The boost virus: core/savestate.cpp
# These are for retarded scripting backdoor...
# core/packet.cpp core/rpc_server.cpp core/server.cpp core/udp_server.cpp

## GPU
SRC += video/command_processor.cpp video/debug_utils.cpp video/geometry_pipeline.cpp video/pica.cpp video/primitive_assembly.cpp video/regs.cpp video/renderer_base.cpp video/renderer_opengl/frame_dumper_opengl.cpp video/renderer_opengl/gl_rasterizer.cpp video/renderer_opengl/gl_rasterizer_cache.cpp video/renderer_opengl/gl_resource_manager.cpp video/renderer_opengl/gl_shader_decompiler.cpp video/renderer_opengl/gl_shader_disk_cache.cpp video/renderer_opengl/gl_shader_gen.cpp video/renderer_opengl/gl_shader_manager.cpp video/renderer_opengl/gl_shader_util.cpp video/renderer_opengl/gl_state.cpp video/renderer_opengl/gl_stream_buffer.cpp video/renderer_opengl/gl_surface_params.cpp video/renderer_opengl/gl_vars.cpp video/renderer_opengl/post_processing_opengl.cpp video/renderer_opengl/renderer_opengl.cpp video/renderer_opengl/anime4k_ultrafast.cpp video/renderer_opengl/bicubic.cpp video/renderer_opengl/scale_force.cpp video/renderer_opengl/texture_filterer.cpp video/renderer_opengl/xbrz_freescale.cpp

#temporary, move these back in alphabetical order before merging
SRC += video/renderer_opengl/gl_format_reinterpreter.cpp video/shader.cpp video/shader_interpreter.cpp video/swrasterizer/clipper.cpp video/swrasterizer/framebuffer.cpp video/swrasterizer/lighting.cpp video/swrasterizer/proctex.cpp video/swrasterizer/rasterizer.cpp video/swrasterizer/swrasterizer.cpp video/swrasterizer/texturing.cpp video/etc1.cpp video/texture_decode.cpp video/vertex_loader.cpp video/video_core.cpp

#if(ARCHITECTURE_x86_64)
SRC += video/shader_jit_x64.cpp video/shader_jit_x64_compiler.cpp

# WEBSERVICE
## ENET
#SRC += externals/enet/callbacks.c externals/enet/compress.c externals/enet/host.c externals/enet/list.c externals/enet/packet.c externals/enet/peer.c externals/enet/protocol.c externals/enet/unix.c externals/enet/win32.c
## LURLPARSER
#SRC += externals/lurlparser/LUrlParser.cpp
## NETWORK
#SRC += network/network.cpp network/packet.cpp network/room.cpp network/room_member.cpp network/verify_user.cpp
#SRC += core/announce_multiplayer_session.cpp
## NWM SERVICE
# SRC += core/hle/service/nwm.cpp core/hle/service/nwm_cec.cpp core/hle/service/nwm_ext.cpp core/hle/service/nwm_inf.cpp core/hle/service/nwm_sap.cpp core/hle/service/nwm_soc.cpp core/hle/service/nwm_tst.cpp core/hle/service/nwm_uds.cpp core/hle/service/uds_beacon.cpp core/hle/service/uds_connection.cpp core/hle/service/uds_data.cpp
## MISC
#SRC += webs/announce_room_json.cpp webs/telemetry_json.cpp webs/verify_login.cpp webs/verify_user_jwt.cpp webs/web_backend.cpp
#SRC += dedicated_room/citra-room.cpp

#if (ENABLE_FFMPEG_VIDEO_DUMPER)
# core/dumping/ffmpeg_backend.cpp core/dumping/ffmpeg_backend.h

# Shaders generate?
# video/renderer_opengl/texture_filters/anime4k/refine.frag video/renderer_opengl/texture_filters/anime4k/refine.vert video/renderer_opengl/texture_filters/anime4k/x_gradient.frag video/renderer_opengl/texture_filters/anime4k/y_gradient.frag video/renderer_opengl/texture_filters/anime4k/y_gradient.vert video/renderer_opengl/texture_filters/bicubic/bicubic.frag video/renderer_opengl/texture_filters/scale_force/scale_force.frag video/renderer_opengl/texture_filters/tex_coord.vert video/renderer_opengl/texture_filters/xbrz/xbrz_freescale.frag video/renderer_opengl/texture_filters/xbrz/xbrz_freescale.vert

LIBSRC = ${SRC}

PROGSRC = citra.cpp config.cpp emu_window_sdl2.cpp

include make/exconf.noasm
include make/build

dist-clean: clean

# Objects using the core.
clean-core:
	rm -f citra.o emu_window_sdl2.o audio/dsp_interface.o audio/lle.o audio/hle/decoder.o audio/hle/ffmpeg_decoder.o audio/hle/hle.o core/3dsx.o core/archive_ncch.o core/archive_savedata.o core/archive_selfncch.o core/arm_dynarmic.o core/arm_dyncom.o core/arm_dyncom_interpreter.o core/armstate.o core/cheats.o core/core.o core/custom_tex_cache.o core/elf.o core/gateway_cheat.o core/gdbstub.o core/gpu.o core/loader.o core/memory.o core/movie.o core/ncch_container.o core/ncch.o core/rpc_server.o core/savestate.o core/server.o core/settings.o core/telemetry_session.o core/y2r.o core/frontend/default_applets.o core/frontend/swkbd.o core/hle/applet.o core/hle/erreula.o core/hle/hle_ipc.o core/hle/ipc.o core/hle/memory.o core/hle/mii_selector.o core/hle/mint.o core/hle/mutex.o core/hle/shared_page.o core/hle/svc.o core/hle/swkbd.o core/hle/thread.o core/hle/timer.o core/hle/service/ac.o core/hle/service/act.o core/hle/service/am.o core/hle/service/applet_manager.o core/hle/service/apt.o core/hle/service/archive.o core/hle/service/boss.o core/hle/service/cam.o core/hle/service/cecd.o core/hle/service/cfg.o core/hle/service/cro_helper.o core/hle/service/csnd_snd.o core/hle/service/dlp.o core/hle/service/dsp_dsp.o core/hle/service/err_f.o core/hle/service/extra_hid.o core/hle/service/file.o core/hle/service/frd.o core/hle/service/fs_user.o core/hle/service/gsp.o core/hle/service/gsp_gpu.o core/hle/service/hid.o core/hle/service/http_c.o core/hle/service/ir.o core/hle/service/ir_rst.o core/hle/service/ir_user.o core/hle/service/ldr_ro.o core/hle/service/mic_u.o core/hle/service/mvd.o core/hle/service/ndm_u.o core/hle/service/news.o core/hle/service/nfc.o core/hle/service/nim.o core/hle/service/nim_u.o core/hle/service/ns.o core/hle/service/nwm.o core/hle/service/nwm_uds.o core/hle/service/pm.o core/hle/service/ps_ps.o core/hle/service/ptm.o core/hle/service/pxi.o core/hle/service/qtm.o core/hle/service/service.o core/hle/service/sm.o core/hle/service/soc_u.o core/hle/service/srv.o core/hle/service/ssl_c.o core/hle/service/y2r_u.o
#audio/hle/fdk_decoder.o audio/hle/mediandk_decoder.o audio/hle/wmf_decoder.o
	rm -f video/renderer_opengl/gl_rasterizer.o video/renderer_opengl/gl_rasterizer_cache.o video/renderer_opengl/gl_shader_disk_cache.o video/renderer_opengl/gl_shader_gen.o video/renderer_opengl/gl_shader_manager.o video/renderer_opengl/renderer_opengl.o
	rm -f dedicated_room/citra-room.o
#        rm -f citra_qt/bootmanager.o citra_qt/cheats.o citra_qt/compatdb.o citra_qt/discord_impl.o citra_qt/main.o citra_qt/configuration/configure_audio.o citra_qt/configuration/configure_camera.o citra_qt/configuration/configure_debug.o citra_qt/configuration/configure_enhancements.o citra_qt/configuration/configure_general.o citra_qt/configuration/configure_graphics.o citra_qt/configuration/configure_system.o citra_qt/debugger/registers.o citra_qt/debugger/wait_tree.o citra_qt/debugger/graphics/graphics_cmdlists.o citra_qt/debugger/graphics/graphics_surface.o citra_qt/debugger/ipc/recorder.o

clean-ext:
	rm -f externals/*/*.o
clean-test:
	rm -f audio/*.o audio/*/*.o common/*.o common/*/*.o core/*.o core/*/*.o core/*/*/*.o input/*.o network/*.o video/*.o video/*/*.o
#WEBSERVICE
clean-web:
	rm -f externals/enet/callbacks.o externals/enet/compress.o externals/enet/host.o externals/enet/list.o externals/enet/packet.o externals/enet/peer.o externals/enet/protocol.o externals/enet/unix.o externals/enet/win32.o externals/lurlparser/LUrlParser.o network/network.o network/packet.o network/room.o network/room_member.o network/verify_user.o core/announce_multiplayer_session.o core/hle/service/nwm.o core/hle/service/nwm_cec.o core/hle/service/nwm_ext.o core/hle/service/nwm_inf.o core/hle/service/nwm_sap.o core/hle/service/nwm_soc.o core/hle/service/nwm_tst.o core/hle/service/nwm_uds.o core/hle/service/uds_beacon.o core/hle/service/uds_connection.o core/hle/service/uds_data.o webs/announce_room_json.o webs/telemetry_json.o webs/verify_login.o webs/verify_user_jwt.o webs/web_backend.o dedicated_room/citra-room.o

${PROGS}: citra.o config.o emu_window_sdl2.o

include make/pack
include make/rules
#include make/thedep
