#ifndef COREGL_FASTPATH_H
#define COREGL_FASTPATH_H

#include "../coregl_module.h"

#ifndef COREGL_USE_MODULE_FASTPATH
#error "COREGL_USE_MODULE_FASTPATH must defined!!!"
#endif

#define MY_MODULE_ID              COREGL_MODULE_FASTPATH
#define MY_MODULE_TSTATE         Fastpath_ThreadState

#include "../../coregl_internal.h"
#include "../../coregl_export.h"

#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     extern RET_TYPE (*_orig_fastpath_##FUNC_NAME) PARAM_LIST;
# include "../../headers/sym_gl1.h"
# include "../../headers/sym_gl2.h"
# include "../../headers/sym_gl_common.h"
# include "../../headers/sym_egl.h"
#undef _COREGL_SYMBOL

// Symbol definition for fastpath
#define _COREGL_SYMBOL(RET_TYPE, FUNC_NAME, PARAM_LIST)     extern RET_TYPE (fastpath_##FUNC_NAME) PARAM_LIST;
# include "../../headers/sym_gl1.h"
# include "../../headers/sym_gl2.h"
# include "../../headers/sym_gl_common.h"
# include "../../headers/sym_egl.h"
#undef _COREGL_SYMBOL

#define COREGL_FASTPATH_TRACE_ALL

#ifdef COREGL_FASTPATH_TRACE_ALL
#define COREGL_FASTPATH_TRACE_CONTEXT_INFO   // Context state & thread state & Glue-context info
#define COREGL_FASTPATH_TRACE_STATE_INFO     // Glue-context state info
#endif


#define _COREGL_FASTPATH_FUNC_BEGIN()

#define _COREGL_FASTPATH_FUNC_END()


#define FLAG_BIT_0      0x01
#define FLAG_BIT_1      0x02
#define FLAG_BIT_2      0x04
#define FLAG_BIT_3      0x08
#define FLAG_BIT_4      0x10
#define FLAG_BIT_5      0x20
#define FLAG_BIT_6      0x40
#define FLAG_BIT_7      0x80

#define MAX_TEXTURE_UNITS 96
#define MAX_VERTEX_ATTRIBS 64
#define MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 64
#define MAX_UNIFORM_BUFFER_BINDINGS 72
#define MAX_SHADER_STORAGE_BUFFER_BINDINGS 70
#define MAX_ATOMIC_COUNTER_BUFFER_BINDING 8

typedef enum _Fastpath_Opt_Flag {
	FP_UNKNOWN_PATH,
	FP_NORMAL_PATH,
	FP_FAST_PATH
} Fastpath_Opt_Flag;

extern Fastpath_Opt_Flag   fp_opt;
extern int                 debug_nofp;

typedef struct _GLContextState {
	int                      ref_count;
	GLContext               *rctx;
	GLDisplay               *rdpy;
	void                    *data;
} GLContextState;

typedef struct _Fastpath_ThreadState {
	EGLenum                  binded_api;
	GLContextState          *cstate;
	GLSurface               *rsurf_draw;
	GLSurface               *rsurf_read;
} Fastpath_ThreadState;

typedef struct _GLContext_List {
	void                    *option;
	int                      option_len;
	int                      thread_id;
	GLContextState          *cstate;
	struct _GLContext_List *next;
} GLContext_List;

extern GLContext_List      *glctx_list;

extern Mutex                ctx_list_access_mutex;

typedef struct {
	GLuint   tex_id;
} GL_Texture_State;

typedef struct {
	GLboolean    modified;
	GLboolean    enabled;
	GLuint       buf_id;
	GLint        size;
	GLenum       type;
	GLboolean    normalized;
	GLsizei      stride;
	const void  *pointer;
} GL_Vertex_Array_State;

typedef struct {
	GLboolean   modified;
	GLfloat     value[4];
} GL_Vertex_Attrib;


#define MAGIC_GLFAST                0x73777770
#define GL_OBJECT_HASH_BASE        (1 << 10)

#define GL_OBJECT_ID_LIMIT         0xFFFFFF

#define GL_Object_Type                     int
#define GL_OBJECT_TYPE_QUERY               0x0
#define GL_OBJECT_TYPE_TEXTURE             0x1000000
#define GL_OBJECT_TYPE_BUFFER              0x2000000
#define GL_OBJECT_TYPE_FRAMEBUFFER         0x3000000
#define GL_OBJECT_TYPE_RENDERBUFFER        0x4000000
#define GL_OBJECT_TYPE_PROGRAM             0x5000000
#define GL_OBJECT_TYPE_VERTEXARRAY         0x6000000
#define GL_OBJECT_TYPE_SAMPLER             0x7000000
#define GL_OBJECT_TYPE_TRANSFORMFEEDBACK   0x8000000
#define GL_OBJECT_TYPE_PROGRAMPIPELINE     0x9000000
#define GL_OBJECT_TYPE_UNKNOWN             0xFFFFFFF

typedef struct _GL_Object {
	GLuint                            id;
	GLuint                            real_id;
	GLint                             ref_count;
	GLvoid                            *tag;

	struct _GL_Shared_Object_State  *parent;
} GL_Object;

typedef struct _GL_Object_Hash {
	GLuint                           hash_key;
	GL_Object                        *item;
	struct _GL_Object_Hash          *next;
} GL_Object_Hash;

typedef struct _GL_Object_Hash_Base {
	GLuint                            hash_size;
	GLuint                            item_size;
	GLuint                            last_id;
	GLuint                            is_looped;
	GL_Object_Hash                  **hash_field;
} GL_Object_Hash_Base;

typedef struct _GL_Shared_Object_State {
	Mutex                    access_mutex;
	Mutex                    real_access_mutex;
	int                      ref_count;
	General_Trace_List      *using_gctxs;

	GL_Object_Hash_Base      texture;
	GL_Object_Hash_Base      buffer;
	GL_Object_Hash_Base      renderbuffer;
	GL_Object_Hash_Base      program;
	GL_Object_Hash_Base      sampler;
	GL_Object_Hash_Base      programpipeline;

	GL_Object_Hash_Base      texture_real;
	GL_Object_Hash_Base      buffer_real;
	GL_Object_Hash_Base      renderbuffer_real;
	GL_Object_Hash_Base      program_real;
	GL_Object_Hash_Base      sampler_real;

	GL_Object_Hash_Base      programpipeline_real;
} GL_Shared_Object_State;

typedef struct _GL_Object_State {
	GL_Shared_Object_State  *shared;

	GL_Object_Hash_Base      query;
	GL_Object_Hash_Base      framebuffer;
	GL_Object_Hash_Base      vertexarray;
	GL_Object_Hash_Base      transformfeedback;

	GL_Object_Hash_Base      query_real;
	GL_Object_Hash_Base      framebuffer_real;
	GL_Object_Hash_Base      vertexarray_real;
	GL_Object_Hash_Base      transformfeedback_real;
} GL_Object_State;

typedef struct _GLGlueContext {
	int                     magic;
	int                     initialized;
	int                     surface_attached;

	int							ref_count;
	int							is_destroyed;

	double                  glversion;

	int							used_count;

	GLDisplay              *rdpy;
	GLContextState         *cstate;
	int                     thread_id;

	void                   *real_ctx_option;
	int                     real_ctx_option_len;

	void                   *real_ctx_sharable_option;
	int                     real_ctx_sharable_option_len;




	unsigned char           _bind_flag1;
#define _BIND_FLAG1_BIT_gl_array_buffer_binding               FLAG_BIT_0
#define _BIND_FLAG1_BIT_gl_element_array_buffer_binding       FLAG_BIT_1
#define _BIND_FLAG1_BIT_gl_framebuffer_binding                FLAG_BIT_2
#define _BIND_FLAG1_BIT_gl_renderbuffer_binding               FLAG_BIT_3
#define _BIND_FLAG1_BIT_gl_framebuffer_binding_read           FLAG_BIT_4
#define _BIND_FLAG1_BIT_gl_framebuffer_binding_draw           FLAG_BIT_5

	unsigned char           _bind_flag2;
#define _BIND_FLAG2_BIT_gl_copy_read_buffer_binding           FLAG_BIT_0
#define _BIND_FLAG2_BIT_gl_copy_write_buffer_binding          FLAG_BIT_1
#define _BIND_FLAG2_BIT_gl_pixel_pack_buffer_binding          FLAG_BIT_2
#define _BIND_FLAG2_BIT_gl_pixel_unpack_buffer_binding        FLAG_BIT_3
#define _BIND_FLAG2_BIT_gl_transform_feedback_buffer_binding  FLAG_BIT_4
#define _BIND_FLAG2_BIT_gl_uniform_buffer_binding             FLAG_BIT_5

	unsigned char           _bind_flag3;
#define _BIND_FLAG3_BIT_gl_shader_storage_buffer_binding      FLAG_BIT_0
#define _BIND_FLAG3_BIT_gl_atomic_counter_buffer_binding      FLAG_BIT_1
#define _BIND_FLAG3_BIT_gl_draw_indirect_buffer_binding       FLAG_BIT_2
#define _BIND_FLAG3_BIT_gl_dispatch_indirect_buffer_binding   FLAG_BIT_3
#define _BIND_FLAG3_BIT_gl_texture_buffer_binding             FLAG_BIT_4

	unsigned char           _bind_flag4;
#define _BIND_FLAG4_BIT_gl_transform_feedback_buffer_range_binding  FLAG_BIT_0
#define _BIND_FLAG4_BIT_gl_uniform_buffer_range_binding             FLAG_BIT_1
#define _BIND_FLAG4_BIT_gl_shader_storage_buffer_range_binding      FLAG_BIT_2
#define _BIND_FLAG4_BIT_gl_atomic_counter_buffer_range_binding	    FLAG_BIT_3

	unsigned char           _enable_flag1;
#define _ENABLE_FLAG1_BIT_gl_blend        FLAG_BIT_0
#define _ENABLE_FLAG1_BIT_gl_cull_face    FLAG_BIT_1
#define _ENABLE_FLAG1_BIT_gl_depth_test   FLAG_BIT_2
#define _ENABLE_FLAG1_BIT_gl_dither       FLAG_BIT_3

	unsigned char           _enable_flag2;
#define _ENABLE_FLAG2_BIT_gl_polygon_offset_fill       FLAG_BIT_0
#define _ENABLE_FLAG2_BIT_gl_sample_alpha_to_coverage  FLAG_BIT_1
#define _ENABLE_FLAG2_BIT_gl_sample_coverage           FLAG_BIT_2
#define _ENABLE_FLAG2_BIT_gl_scissor_test              FLAG_BIT_3
#define _ENABLE_FLAG2_BIT_gl_stencil_test              FLAG_BIT_4
#define _ENABLE_FLAG2_BIT_gl_sample_shading_oes        FLAG_BIT_5
#define _ENABLE_FLAG2_BIT_gl_sample_mask               FLAG_BIT_6

	unsigned char           _enable_flag3;
#define _ENABLE_FLAG3_BIT_gl_primitive_restart_fixed_index  FLAG_BIT_0
#define _ENABLE_FLAG3_BIT_gl_rasterizer_discard             FLAG_BIT_1
#define _ENABLE_FLAG3_BIT_gl_blend_advanced_coherent_khr    FLAG_BIT_2

	unsigned char           _clear_flag1;
#define _CLEAR_FLAG1_BIT_gl_viewport            FLAG_BIT_0
#define _CLEAR_FLAG1_BIT_gl_current_program     FLAG_BIT_1
#define _CLEAR_FLAG1_BIT_gl_color_clear_value   FLAG_BIT_2

	unsigned char           _clear_flag2;
#define _CLEAR_FLAG2_BIT_gl_color_writemask     FLAG_BIT_0
#define _CLEAR_FLAG2_BIT_gl_depth_range         FLAG_BIT_1
#define _CLEAR_FLAG2_BIT_gl_depth_clear_value   FLAG_BIT_2
#define _CLEAR_FLAG2_BIT_gl_depth_func          FLAG_BIT_3
#define _CLEAR_FLAG2_BIT_gl_depth_writemask     FLAG_BIT_4
#define _CLEAR_FLAG2_BIT_gl_cull_face_mode      FLAG_BIT_5

	unsigned char           _tex_flag1;
#define _TEX_FLAG1_BIT_gl_active_texture         FLAG_BIT_0
#define _TEX_FLAG1_BIT_gl_generate_mipmap_hint   FLAG_BIT_1
#define _TEX_FLAG1_BIT_gl_tex_2d_state           FLAG_BIT_2
#define _TEX_FLAG1_BIT_gl_tex_3d_state           FLAG_BIT_3
#define _TEX_FLAG1_BIT_gl_tex_2d_array_state     FLAG_BIT_4
#define _TEX_FLAG1_BIT_gl_tex_cube_state         FLAG_BIT_5
#define _TEX_FLAG1_BIT_gl_tex_external_oes_state FLAG_BIT_6
#define _TEX_FLAG1_BIT_gl_client_active_texture  FLAG_BIT_7

	unsigned char           _tex_flag2;
#define _TEX_FLAG2_BIT_gl_tex_buffer_ext_state               FLAG_BIT_0
#define _TEX_FLAG2_BIT_gl_tex_2d_multisample_state           FLAG_BIT_1
#define _TEX_FLAG2_BIT_gl_tex_2d_multisample_array_oes_state FLAG_BIT_2
#define _TEX_FLAG2_BIT_gl_tex_cube_map_array_ext_state       FLAG_BIT_3

	unsigned char           _blend_flag;
#define _BLEND_FLAG_BIT_gl_blend_color           FLAG_BIT_0
#define _BLEND_FLAG_BIT_gl_blend_src_rgb         FLAG_BIT_1
#define _BLEND_FLAG_BIT_gl_blend_src_alpha       FLAG_BIT_2
#define _BLEND_FLAG_BIT_gl_blend_dst_rgb         FLAG_BIT_3
#define _BLEND_FLAG_BIT_gl_blend_dst_alpha       FLAG_BIT_4
#define _BLEND_FLAG_BIT_gl_blend_equation_rgb    FLAG_BIT_5
#define _BLEND_FLAG_BIT_gl_blend_equation_alpha  FLAG_BIT_6

	unsigned char           _stencil_flag1;
#define _STENCIL_FLAG1_BIT_gl_stencil_func              FLAG_BIT_0
#define _STENCIL_FLAG1_BIT_gl_stencil_ref               FLAG_BIT_1
#define _STENCIL_FLAG1_BIT_gl_stencil_value_mask        FLAG_BIT_2
#define _STENCIL_FLAG1_BIT_gl_stencil_fail              FLAG_BIT_3
#define _STENCIL_FLAG1_BIT_gl_stencil_pass_depth_fail   FLAG_BIT_4
#define _STENCIL_FLAG1_BIT_gl_stencil_pass_depth_pass   FLAG_BIT_5
#define _STENCIL_FLAG1_BIT_gl_stencil_writemask         FLAG_BIT_6

	unsigned char           _stencil_flag2;
#define _STENCIL_FLAG2_BIT_gl_stencil_back_func              FLAG_BIT_0
#define _STENCIL_FLAG2_BIT_gl_stencil_back_ref               FLAG_BIT_1
#define _STENCIL_FLAG2_BIT_gl_stencil_back_value_mask        FLAG_BIT_2
#define _STENCIL_FLAG2_BIT_gl_stencil_back_fail              FLAG_BIT_3
#define _STENCIL_FLAG2_BIT_gl_stencil_back_pass_depth_fail   FLAG_BIT_4
#define _STENCIL_FLAG2_BIT_gl_stencil_back_pass_depth_pass   FLAG_BIT_5
#define _STENCIL_FLAG2_BIT_gl_stencil_back_writemask         FLAG_BIT_6
#define _STENCIL_FLAG2_BIT_gl_stencil_clear_value            FLAG_BIT_7

	unsigned char           _pixel_flag1;
#define _PIXEL_FLAG1_BIT_gl_pack_row_length      FLAG_BIT_0
#define _PIXEL_FLAG1_BIT_gl_pack_skip_rows       FLAG_BIT_1
#define _PIXEL_FLAG1_BIT_gl_pack_skip_pixels     FLAG_BIT_2
#define _PIXEL_FLAG1_BIT_gl_pack_alignment       FLAG_BIT_3

	unsigned char           _pixel_flag2;
#define _PIXEL_FLAG2_BIT_gl_unpack_row_length      FLAG_BIT_0
#define _PIXEL_FLAG2_BIT_gl_unpack_skip_rows       FLAG_BIT_1
#define _PIXEL_FLAG2_BIT_gl_unpack_skip_pixels     FLAG_BIT_2
#define _PIXEL_FLAG2_BIT_gl_unpack_alignment       FLAG_BIT_3
#define _PIXEL_FLAG2_BIT_gl_unpack_image_height    FLAG_BIT_4
#define _PIXEL_FLAG2_BIT_gl_unpack_skip_images     FLAG_BIT_5

	unsigned char           _misc_flag1;
#define _MISC_FLAG1_BIT_gl_front_face                        FLAG_BIT_0
#define _MISC_FLAG1_BIT_gl_line_width                        FLAG_BIT_1
#define _MISC_FLAG1_BIT_gl_polygon_offset_factor             FLAG_BIT_2
#define _MISC_FLAG1_BIT_gl_polygon_offset_units              FLAG_BIT_3
#define _MISC_FLAG1_BIT_gl_sample_coverage_value             FLAG_BIT_4
#define _MISC_FLAG1_BIT_gl_sample_coverage_invert            FLAG_BIT_5
#define _MISC_FLAG1_BIT_gl_fragment_shader_derivative_hint   FLAG_BIT_6

	unsigned char           _misc_flag2;
#define _MISC_FLAG2_BIT_gl_scissor_box                       FLAG_BIT_0

	unsigned char           _misc_flag3;
#define _MISC_FLAG3_BIT_gl_read_buffer                       FLAG_BIT_0
#define _MISC_FLAG3_BIT_gl_draw_buffers                      FLAG_BIT_1
#define _MISC_FLAG3_BIT_gl_vertex_array_binding              FLAG_BIT_2
#define _MISC_FLAG3_BIT_gl_transform_feedback_binding        FLAG_BIT_3
#define _MISC_FLAG3_BIT_gl_transform_feedback                FLAG_BIT_4
#define _MISC_FLAG3_BIT_gl_draw_range_elements               FLAG_BIT_5
#define _MISC_FLAG3_BIT_gl_program_pipeline_binding          FLAG_BIT_6

	unsigned char           _vattrib_flag;
#define _VATTRIB_FLAG_BIT_gl_vertex_attrib_value             FLAG_BIT_0
#define _VATTRIB_FLAG_BIT_gl_vertex_array                    FLAG_BIT_1

	GL_Object_State         ostate;

	GLenum                  gl_error;

	// General state
#define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)     TYPE *NAME;
# include "coregl_fastpath_state.h"
#undef GLUE_STATE
#define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)     GLboolean *NAME##_updated;
# include "coregl_fastpath_state.h"
#undef GLUE_STATE
#define GLUE_STATE(TYPE, NAME, SIZE, ARRAY_SIZE, DEFAULT_STMT, GET_STMT)     unsigned char NAME##_used;
# include "coregl_fastpath_state.h"
#undef GLUE_STATE

} GLGlueContext;

typedef struct _GLGlueContext_List {
	GLGlueContext              *gctx;
	struct _GLGlueContext_List *prev;
	struct _GLGlueContext_List *next;
} GLGlueContext_List;

extern GLGlueContext_List *gctx_list;

extern GLGlueContext *initial_ctx;

extern void                init_modules_fastpath();
extern void                deinit_modules_fastpath();
extern void                init_modules_tstate_fastpath(GLThreadState *tstate);
extern void                deinit_modules_tstate_fastpath(
	GLThreadState *tstate);

extern void                fastpath_apply_overrides();
extern void                fastpath_apply_overrides_egl(int enable);
extern void                fastpath_apply_overrides_gl(int enable);

extern int                 fastpath_init_context_states(GLGlueContext *ctx);
extern int                 fastpath_make_context_current(GLGlueContext *oldctx,
		GLGlueContext *newctx);

#ifdef COREGL_FASTPATH_TRACE_STATE_INFO
extern void                fastpath_dump_context_states(GLGlueContext *ctx,
		int force_output);
#endif // COREGL_FASTPATH_TRACE_STATE_INFO

// Context state functions
extern int                 fastpath_add_context_state_to_list(const void *data,
		const int datalen, GLContextState *cstate, Mutex *mtx);
extern int                 fastpath_remove_context_states_from_list(
	GLContextState *cstate, Mutex *mtx);
extern GLContextState     *fastpath_get_context_state_from_list(
	const void *data, const int datalen, Mutex *mtx);

// Shared object state functions
extern void                fastpath_ostate_init(GL_Object_State *ostate);
extern void                fastpath_sostate_init(GL_Shared_Object_State
		*ostate);
extern void                fastpath_ostate_deinit(GL_Object_State *ostate);
extern void                fastpath_sostate_deinit(GL_Shared_Object_State
		*ostate);
extern GLuint              fastpath_ostate_create_object(
	GL_Object_State *ostate, GL_Object_Type type, GLuint name);
extern GLuint              fastpath_ostate_remove_object(
	GL_Object_State *ostate, GL_Object_Type type, GLuint glue_name);
extern GLuint              fastpath_ostate_get_object(GL_Object_State *ostate,
		GL_Object_Type type, GLuint name);
extern GLuint              fastpath_ostate_find_object(GL_Object_State *ostate,
		GL_Object_Type type, GLuint real_name);
extern GLint               fastpath_ostate_use_object(GL_Object_State *ostate,
		GL_Object_Type type, GLuint glue_name);
extern GLint               fastpath_ostate_set_object_tag(
	GL_Object_State *ostate, GL_Object_Type type, GLuint glue_name, GLvoid *tag);
extern GLvoid             *fastpath_ostate_get_object_tag(
	GL_Object_State *ostate, GL_Object_Type type, GLuint glue_name);

// GL context management functions
extern void                fastpath_release_gl_context(GLGlueContext *gctx);

// State query functions
extern void                fastpath_state_get_draw_buffers(GLenum *params);

#endif // COREGL_FASTPATH_H

