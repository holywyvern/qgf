#include "qgf/tools/openGL.h"
#include "qgf/tools/array.h"

#ifdef _WIN32
#include "GL/glew.h"
#include "GL/wglew.h"
#endif

#include <gl/GL.h>
#include <gl/GLU.h>

#define PI 3.141592654

const GLchar* SPRITE_FRAGMENT_SHADER_SRC = "#version 130\n"
"uniform sampler2D texCMYK;\n"
"uniform sampler2D texRGB;\n"
"uniform float angle;\n"
"uniform float amp;\n"
"uniform float length;\n"
"uniform float width;\n"
"uniform float height;\n"
"uniform float tr;\n"
"uniform float tg;\n"
"uniform float tb;\n"
"uniform float gray;\n"
"uniform float opacity;\n"
"const float PI = 3.1415;\n"
"void main()\n"
"{\n"
"    float ts = gl_TexCoord[0].t;\n"
"    float ag = 1 - gray;\n"
"    float w  = PI * ts * width / length;\n"
"    vec2 mod_texcoord = gl_TexCoord[0].st + vec2(amp * sin(w + angle) / width, 0);\n"
"    vec4 tp = texture2D(texRGB, mod_texcoord);\n"
"    vec4 tone = vec4(tr, tg, tb, 0);\n"
"    float avg = 0.2126 * (tp[0]) + 0.7152 * (tp[1]) + 0.0722 * (tp[2]);\n"
"    vec4 gs = vec4(avg, avg, avg, tp[3]) * gray;\n"
"    gl_FragColor = (tp * (1 - gray) + tone + gs) * gl_Color;\n"
"}\0";

const GLchar* SPRITE_VERTEX_SHADER_SRC = "#version 130\n"
"void main()"
"{"
"   gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;"
"   gl_TexCoord[0] = gl_MultiTexCoord0;"
"   gl_FrontColor = gl_Color;"
"   gl_BackColor = gl_Color;" 
"}\0";

static GLuint qgf_sprite_vertexShader;
static GLuint qgf_sprite_fragmentShader;
static GLuint qgf_sprite_shaderProgram;
static GLuint texCMYK;

char 
qgf_tools_openGL_checkShader(GLuint shader) {
    GLint n;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &n);
    if( n == GL_FALSE ) {
        GLchar *info_log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &n);
        info_log = (GLchar*)malloc(n);
        glGetShaderInfoLog(shader, n, &n, info_log);
        fprintf(stderr, "Shader compilation failed: %*s\n", n, info_log);
        free(info_log);
        return 0;
    }
	return 1;
}

static 
void qgf_tools_openGL_bindTexture(const GLchar* sampler_name, GLuint texture_unit, GLuint texture, GLuint shaderProgram) {
        glActiveTexture(GL_TEXTURE0 + texture_unit); 
        glBindTexture(GL_TEXTURE_2D, texture);
        GLuint loc_sampler = glGetUniformLocation(shaderProgram, sampler_name);
        glUniform1i(loc_sampler, texture_unit);
}

#define TEX_CMYK_WIDTH 2
#define TEX_CMYK_HEIGHT 2
GLubyte textureDataCMYK[TEX_CMYK_WIDTH * TEX_CMYK_HEIGHT][3] = {
    {0x00, 0xff, 0xff}, {0xff, 0x00, 0xff},
    {0xff, 0xff, 0x00}, {0x00, 0x00, 0x00}
};

char
qgf_tools_openGL_initShaders() {
    qgf_sprite_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(qgf_sprite_vertexShader, 1, (const GLchar**)&SPRITE_VERTEX_SHADER_SRC, NULL);
	glCompileShader(qgf_sprite_vertexShader);
	if (!qgf_tools_openGL_checkShader(qgf_sprite_vertexShader)) return 0;
	qgf_sprite_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(qgf_sprite_fragmentShader, 1, (const GLchar**)&SPRITE_FRAGMENT_SHADER_SRC, NULL);
    glCompileShader(qgf_sprite_fragmentShader);
	if (!qgf_tools_openGL_checkShader(qgf_sprite_fragmentShader)) return 0;
    qgf_sprite_shaderProgram = glCreateProgram();
    glAttachShader(qgf_sprite_shaderProgram, qgf_sprite_vertexShader);
    glAttachShader(qgf_sprite_shaderProgram, qgf_sprite_fragmentShader);
	glLinkProgram(qgf_sprite_shaderProgram);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glLinkProgram(qgf_sprite_shaderProgram);
    glGenTextures(1, &texCMYK);
    glBindTexture(GL_TEXTURE_2D, texCMYK);
    glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGB8, TEX_CMYK_WIDTH, TEX_CMYK_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, textureDataCMYK);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	return 1;
}

void 
qgf_tools_openGL_initWindow(long width, long height) {
	glViewport(0, 0, width, height);
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glClearColor(
				(GLclampf)(0.0f),
				(GLclampf)(0.0f),
				(GLclampf)(0.0f),
				(GLclampf)(0.0f)
	);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_BLEND);

	glClearColor(
				(GLclampf)(0.0f),
				(GLclampf)(0.0f),
				(GLclampf)(0.0f),
				(GLclampf)(1.0f)
	);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT);    
}

void 
qgf_tools_openGL_updateWindow(long width, long height, TColor* background) {
	glFinish();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(
			(GLclampf)background->red   / 255.0f,
			(GLclampf)background->green / 255.0f,
			(GLclampf)background->blue  / 255.0f,
			(GLclampf)background->alpha / 255.0f
	);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT);  
	glFlush();
}

void
qgf_tools_openGL_finishUpdateWindow() {
	glFinish();
}

void 
qgf_tools_openGL_drawQuad(float corners[4][2], float width, float height, float zoom_x, float zoom_y) {
	glBegin(GL_QUADS);
		glTexCoord2f(corners[0][0], corners[0][1]); glVertex2f(0.0f, 0.0f);
		glTexCoord2f(corners[1][0], corners[1][1]); glVertex2f(width * zoom_x, 0.0f);
		glTexCoord2f(corners[2][0], corners[2][1]); glVertex2f(width * zoom_x, height * zoom_y);
		glTexCoord2f(corners[3][0], corners[3][1]); glVertex2f(0.0f, height * zoom_y);
	glEnd();
}

uint32_t* 
qf_tools_openGL_makePixelsFromRect(TRect* rect, TBitmap* bitmap) {
	return 0;
}

void
qgf_tools_openGL_bind_texture(TViewport* view) {
	if (view->flash_texture > 0) return;
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	uint32_t pixels[4] = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

	glGenTextures(1, &(view->flash_texture));

	glBindTexture(GL_TEXTURE_2D, view->flash_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelTransferf(GL_RED_BIAS, 255.0f);
	glPixelTransferf(GL_GREEN_BIAS, 255.0f);
	glPixelTransferf(GL_BLUE_BIAS, 255.0f);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(pixels[0]));
	glPixelTransferf(GL_RED_BIAS, 0.0f);
	glPixelTransferf(GL_GREEN_BIAS, 0.0f);
	glPixelTransferf(GL_BLUE_BIAS, 0.0f);

}

void 
qgf_tools_openGL_bind_texture(TBitmap* bmp) {
			glEnable(GL_TEXTURE_2D);

			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();

			if (bmp->texture > 0) glDeleteTextures(1, &(bmp->texture));
			glGenTextures(1, &(bmp->texture));

			glBindTexture(GL_TEXTURE_2D, bmp->texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTexImage2D(GL_TEXTURE_2D, 0, 4, bmp->width, bmp->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(bmp->pixels[0]));
			glPixelTransferf(GL_RED_BIAS, 0.0f);
			glPixelTransferf(GL_GREEN_BIAS, 0.0f);
			glPixelTransferf(GL_BLUE_BIAS, 0.0f);

			glEnable(GL_TEXTURE_2D);

			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();

			if (bmp->flash_texture > 0) glDeleteTextures(1, &(bmp->flash_texture));
			glGenTextures(1, &(bmp->flash_texture));

			glBindTexture(GL_TEXTURE_2D, bmp->flash_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glPixelTransferf(GL_RED_BIAS, 255.0f);
			glPixelTransferf(GL_GREEN_BIAS, 255.0f);
			glPixelTransferf(GL_BLUE_BIAS, 255.0f);

			glTexImage2D(GL_TEXTURE_2D, 0, 4, bmp->width, bmp->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(bmp->pixels[0]));
			glPixelTransferf(GL_RED_BIAS, 0.0f);
			glPixelTransferf(GL_GREEN_BIAS, 0.0f);
			glPixelTransferf(GL_BLUE_BIAS, 0.0f);


}

void
rb_cSprite_adjustCorners(float corners[4][2], TSprite* obj, TBitmap* bmp, TRect* src_rect) {
	float rx = src_rect->x / (float)bmp->width;
	float ry = src_rect->y / (float)bmp->height;
	float rw = src_rect->width / (float)bmp->width;
	float rh = src_rect->height / (float)bmp->height;
	corners[0][0] = rx;			corners[0][1] = ry;
	corners[1][0] = rx + rw;	corners[1][1] = ry;
	corners[2][0] = rx + rw;	corners[2][1] = ry + rh;
	corners[3][0] = rx;			corners[3][1] = ry + rh;
	if (obj->mirror_x && obj->mirror_y) {
		corners[0][0] = 1.0f - corners[0][0]; corners[0][1] = 1.0f - corners[0][1];
		corners[1][0] = 1.0f - corners[1][0]; corners[1][1] = 1.0f - corners[1][1];
		corners[2][0] = 1.0f - corners[2][0]; corners[2][1] = 1.0f - corners[2][1];
		corners[3][0] = 1.0f - corners[3][0]; corners[3][1] = 1.0f - corners[3][1];
	} else if (obj->mirror_x) {
		corners[0][0] = 1.0f - corners[0][0];
		corners[1][0] = 1.0f - corners[1][0];
		corners[2][0] = 1.0f - corners[2][0];
		corners[3][0] = 1.0f - corners[3][0];
	} else if (obj->mirror_y) {
		corners[0][1] = 1.0f - corners[0][1];
		corners[1][1] = 1.0f - corners[1][1];
		corners[2][1] = 1.0f - corners[2][1];
		corners[3][1] = 1.0f - corners[3][1];
	}
}

void
qgf_tools_openGL_selectBlendType(int blend_type) {
	glEnable(GL_BLEND);
	switch (blend_type) {
	case 1:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	case 2:
		glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_COLOR);
		break;
	default:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void
qgf_sprite_selectColorWithTint(TSprite* obj, TTone* tone) {
	float alpha = obj->opacity / 255.0f;
	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	glBlendColor(1.0f, 1.0f, 1.0f, alpha);
}

void
qgf_tools_openGL_adjustSpriteColor(TColor* c, TSprite* obj, TTone* tone) {
	TColor* color;
	if (obj->flash_duration > 0 && RTEST(obj->flash_color)) {
		Data_Get_Struct(obj->flash_color, TColor, color);
		c->red    = color->red   / 255.0f;
		c->green  = color->green / 255.0f;
		c->blue   = color->blue  / 255.0f;
		c->alpha  = color->alpha / 255.0f;
		c->alpha  = color->alpha / 255.0f;
	} else if (RTEST(obj->color)) {
		Data_Get_Struct(obj->color, TColor, color);
		c->red    = color->red / 255.0f;
		c->green  = color->green / 255.0f;
		c->blue   = color->blue / 255.0f;
		c->alpha  = color->alpha / 255.0f;
	}
}

void 
rb_cSprite_draw(TSprite* obj, long gw, long gh) {
	TRect* src_rect;
	TBitmap* bmp;
	TColor* color;
	TTone* tone;
	tone = NULL;

	if (!RTEST(obj->bitmap) || !RTEST(obj->src_rect)) {
		return;
	}
	if (obj->zoom_x <= 0 || obj->zoom_y <= 0) {
		return;
	}
	Data_Get_Struct(obj->src_rect, TRect, src_rect);
	if (src_rect->width == 0 || src_rect->height == 0) {
		return;
	}
	Data_Get_Struct(obj->bitmap, TBitmap, bmp);
	if (BITMAP_CHANGED_P(bmp)) {
		qgf_tools_openGL_bind_texture(bmp);
		bmp->flags = bmp->flags & ~BITMAP_FLAG_CHANGED;
	}
	if (RTEST(obj->tone)) {
		Data_Get_Struct(obj->tone, TTone, tone);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, gw, gh);
	glUseProgram(qgf_sprite_shaderProgram);
	float tr = 0.0f;
	float tg = 0.0f;
	float tb = 0.0f;
	float gray = 0.0f;
	if (tone) {
		tr = tone->red / 255.0f;
		tg = tone->green / 255.0f;
		tb = tone->blue / 255.0f;
		gray = tone->gray / 255.0f;
	}

	glEnable(GL_TEXTURE_2D);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "angle"), obj->wave_phase * PI / 180.0f);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "amp"), obj->wave_amp);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "length"), obj->wave_length);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "width"), src_rect->width);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "height"), src_rect->height);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tr"), tr);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tg"), tg);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tb"), tb);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "gray"), gray);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "opacity"), obj->opacity / 255.0f);

	qgf_tools_openGL_bindTexture("texRGB", 0, bmp->texture, qgf_sprite_shaderProgram);
	glTranslatef(obj->x, obj->y, 0.0f);

	if (RTEST(obj->viewport)) {
		TZObject* z;
		TViewport* viewport;
		TRect* viewport_rect;
		Data_Get_Struct(obj->viewport, TZObject, z);
		viewport = (TViewport*)z->object;
		Data_Get_Struct(viewport->rect, TRect, viewport_rect);
		float vrx = (float)(viewport_rect->x - viewport->ox);
		float vry = (float)(viewport_rect->y - viewport->oy);
		glTranslatef(vrx, vry, 0.0f);
		if (RTEST(viewport->tone)) {
			Data_Get_Struct(viewport->tone, TTone, tone);
			tr = tr + tone->red / 255.0f;
			tg = tg + tone->green / 255.0f;
			tb = tb + tone->blue / 255.0f;
			gray = gray + tone->gray / 255.0f;
			glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tr"), tr);
			glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tg"), tg);
			glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tb"), tb);
			glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "gray"), gray);
		}
	}

	glRotatef(-obj->angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(-obj->ox * obj->zoom_x, -obj->oy * obj->zoom_y, 0.0f);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float corners[4][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
	rb_cSprite_adjustCorners(corners, obj, bmp, src_rect);
	qgf_tools_openGL_selectBlendType(obj->blend_type);
	if (obj->bush_depth == 0) {
		qgf_sprite_selectColorWithTint(obj, tone);
		qgf_tools_openGL_drawQuad(corners, src_rect->width, src_rect->height, obj->zoom_x, obj->zoom_y);
	} else {
		if (obj->mirror_y) {
			qgf_sprite_selectColorWithTint(obj, tone);
			glBegin(GL_QUADS);
				glTexCoord2f(corners[0][0], corners[0][1] - obj->bush_depth / (float)bmp->width);	glVertex2f(0.0f, obj->bush_depth * obj->zoom_y);
				glTexCoord2f(corners[1][0], corners[1][1] - obj->bush_depth / (float)bmp->height);	glVertex2f(src_rect->width * obj->zoom_x, obj->bush_depth * obj->zoom_y);
				glTexCoord2f(corners[2][0], corners[2][1]);	glVertex2f(src_rect->width * obj->zoom_x, src_rect->height * obj->zoom_y);
				glTexCoord2f(corners[3][0], corners[3][1]);	glVertex2f(0.0f, src_rect->height * obj->zoom_y);
			glEnd();
			glColor4f(1.0f, 1.0f, 1.0f, (obj->bush_opacity / 255.0f));
			glBegin(GL_QUADS);
				glTexCoord2f(corners[0][0], corners[0][1]);	glVertex2f(0.0f, 0.0f);
				glTexCoord2f(corners[1][0], corners[1][1]);	glVertex2f(bmp->width * obj->zoom_x, 0.0f);
				glTexCoord2f(corners[2][0], corners[0][1] - obj->bush_depth / (float)bmp->height);	glVertex2f(src_rect->width * obj->zoom_x, obj->bush_depth * obj->zoom_y);
				glTexCoord2f(corners[3][0], corners[1][1] - obj->bush_depth / (float)bmp->height);	glVertex2f(0.0f, obj->bush_depth * obj->zoom_y);
			glEnd();
		} else {
			qgf_sprite_selectColorWithTint(obj, tone);
			glBegin(GL_QUADS);
				glTexCoord2f(corners[0][0], corners[0][1]); glVertex2f(0.0f, 0.0f);
				glTexCoord2f(corners[1][0], corners[1][1]); glVertex2f(src_rect->width * obj->zoom_x, 0.0f);
				glTexCoord2f(corners[2][0], corners[2][1] - obj->bush_depth / (float)bmp->height);	glVertex2f(src_rect->width * obj->zoom_x, (src_rect->height -  obj->bush_depth) *  obj->zoom_y);
				glTexCoord2f(corners[3][0], corners[3][1] - obj->bush_depth / (float)src_rect->height);	glVertex2f(0.0f, (src_rect->height -  obj->bush_depth) * obj->zoom_y);
			glEnd();
			glColor4f(1.0f, 1.0f, 1.0f, (obj->bush_opacity / 255.0f));
			glBegin(GL_QUADS);
				glTexCoord2f(corners[0][0], corners[2][1] -  obj->bush_depth / (float)bmp->height);	glVertex2f(0.0f, (src_rect->height - obj->bush_depth) * obj->zoom_y);
				glTexCoord2f(corners[1][0], corners[3][1] -  obj->bush_depth / (float)bmp->height);	glVertex2f(src_rect->width * obj->zoom_x, (src_rect->height - obj->bush_depth) * obj->zoom_y);
				glTexCoord2f(corners[2][0], corners[2][1]);	glVertex2f(src_rect->width * obj->zoom_x, src_rect->height * obj->zoom_y);
				glTexCoord2f(corners[3][0], corners[3][1]);	glVertex2f(0.0f, src_rect->height * obj->zoom_y);
			glEnd();
		}
	}

	
	glBindTexture(GL_TEXTURE_2D, bmp->flash_texture);
	/*
	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
	qgf_sprite_selectColorWithTint(tone);
	qgf_tools_openGL_drawQuad(corners, src_rect->width, src_rect->height, obj->zoom_x, obj->zoom_y);
	*/
	TColor c;
	c.red   = 1.0f;
	c.green = 1.0f;
	c.blue  = 1.0f;
	c.alpha = 1.0f;
	qgf_tools_openGL_adjustSpriteColor(&c, obj, tone);

	if (c.alpha > 0) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(c.red, c.green, c.blue, c.alpha);
		glBlendColor(c.red, c.green, c.blue, c.alpha);
		qgf_tools_openGL_drawQuad(corners, src_rect->width, src_rect->height, obj->zoom_x, obj->zoom_y);
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glUseProgram(0);
}

void
rb_cViewport_draw(TViewport* obj, long gw, long gh) {
	TRect* rect;
	if (!RTEST(obj->rect)) return;
	Data_Get_Struct(obj->rect, TRect, rect);
	qgf_array_sort(&(obj->objects));
	glEnable(GL_SCISSOR_TEST);
	glScissor((GLint)rect->x, (GLint)(gh - (rect->y + rect->height)), (GLsizei)rect->width, (GLsizei)rect->height);
	qgf_tools_openGL_bind_texture(obj);
	for (long i = 0; i < obj->objects.size; i++) {
		TZObject* z;
		Data_Get_Struct(obj->objects.values[i], TZObject, z);
		if (z && z->visible && !z->disposed) {
			z->on_draw(z->object, gw, gh);
		}
	}
	TColor* c;
	if (obj->flash_duration > 0 && RTEST(obj->flash_color)) {
		Data_Get_Struct(obj->flash_color, TColor, c);
	} else if (RTEST(obj->color)) {
		Data_Get_Struct(obj->color, TColor, c);
	}
	
	float corners[4][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

	if (c && c->alpha > 0) {
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, obj->flash_texture);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glViewport(0, 0, gw, gh);
		glTranslatef(rect->x, rect->y, 0.0f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(c->red / 255.0f, c->green / 255.0f, c->blue / 255.0f, c->alpha / 255.0f);
		qgf_tools_openGL_drawQuad(corners, 2, 2, gw / 2.0f, gh / 2.0f);
	}
	glDisable(GL_BLEND);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glUseProgram(0);
}

void 
rb_cPlane_draw(TPlane* obj, long gw, long gh) {
	TBitmap* bmp;
	TColor* color = NULL;
	if (!RTEST(obj->bitmap)) return;
	if (obj->zoom_x <= 0 || obj->zoom_y <= 0) {
		return;
	}
	Data_Get_Struct(obj->bitmap, TBitmap, bmp);
	if (BITMAP_CHANGED_P(bmp)) {
		qgf_tools_openGL_bind_texture(bmp);
		bmp->flags = bmp->flags & ~BITMAP_FLAG_CHANGED;
	}
	if (obj->flash_color > 0 && RTEST(obj->flash_color)) {
		Data_Get_Struct(obj->flash_color, TColor, color);
	} else if (RTEST(obj->color)) {
		Data_Get_Struct(obj->color, TColor, color);
	}
	float tr = 0.0f;
	float tg = 0.0f;
	float tb = 0.0f;
	float gray = 0.0f;
	if (RTEST(obj->tone)) {
		TTone* tone;
		Data_Get_Struct(obj->tone, TTone, tone);
		tr = tone->red / 255.0f;
		tg = tone->green / 255.0f;
		tb = tone->blue / 255.0f;
		gray = tone->gray / 255.0f;
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, gw, gh);
	glUseProgram(qgf_sprite_shaderProgram);
	glEnable(GL_TEXTURE_2D);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "angle"), obj->wave_phase * PI / 180.0f);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "amp"), obj->wave_amp);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "length"), obj->wave_length);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "width"), bmp->width);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "height"), bmp->height);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tr"), tr);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tg"), tg);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tb"), tb);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "gray"), gray);
	glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "opacity"), obj->opacity / 255.0f);
	qgf_tools_openGL_bindTexture("texRGB", 0, bmp->texture, qgf_sprite_shaderProgram);
	float rectw = gw;
	float recth = gh;
	if (RTEST(obj->viewport)) {
		TZObject* z;
		TViewport* viewport;
		TRect* viewport_rect;
		Data_Get_Struct(obj->viewport, TZObject, z);
		viewport = (TViewport*)z->object;
		Data_Get_Struct(viewport->rect, TRect, viewport_rect);
		float vrx = (float)(viewport_rect->x - viewport->ox);
		float vry = (float)(viewport_rect->y - viewport->oy);
		glTranslatef(vrx, vry, 0.0f);
		rectw = viewport_rect->width;
		recth = viewport_rect->height;

		if (RTEST(viewport->tone)) {
			TTone* tone;
			Data_Get_Struct(viewport->tone, TTone, tone);
			tr = tr + tone->red / 255.0f;
			tg = tg + tone->green / 255.0f;
			tb = tb + tone->blue / 255.0f;
			gray = gray + tone->gray / 255.0f;
			glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tr"), tr);
			glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tg"), tg);
			glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "tb"), tb);
			glUniform1f(glGetUniformLocation(qgf_sprite_shaderProgram, "gray"), gray);
		}

	}
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_BLEND);
	float corners[4][2] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};
	qgf_tools_openGL_selectBlendType(obj->blend_type);

	float bmpw = bmp->width * obj->zoom_x;
	float bmph = bmp->height * obj->zoom_y;
	int r_ox = -(int)obj->ox % (int)bmpw;
	int r_oy = -(int)obj->oy % (int)bmph;
	float tilesx = ceil(rectw / bmpw) + ceil(r_ox / bmpw);
	float tilesy = ceil(recth / bmph) + ceil(r_oy / bmph);
	
	while (r_ox > 0) r_ox -= (int)bmpw;
	while (r_oy > 0) r_oy -= (int)bmph;
	glColor4f(1.0f, 1.0f, 1.0f, obj->opacity / 255.0f);
	glBegin(GL_QUADS);
		for (float i = r_ox / bmpw; i < tilesx; i++) {
		for (float j = r_oy / bmph; j < tilesy; j++) {
			glTexCoord2f(0.0f, 0.0f); glVertex2f(i * bmpw, j * bmph);
			glTexCoord2f(1.0f, 0.0f); glVertex2f((i + 1) * bmpw, j * bmph);
			glTexCoord2f(1.0f, 1.0f); glVertex2f((i + 1) * bmpw, (j + 1) * bmph);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(i * bmpw, (j + 1) * bmph);
		}
		}
	glEnd();
	if (color && color->alpha > 0) {
		glColor4f(color->red / 255.0f, color->green / 255.0f, color->blue / 255.0f, color->alpha / 255.0f);
		glBindTexture(GL_TEXTURE_2D, bmp->flash_texture);
		glBegin(GL_QUADS);
			for (float i = r_ox / bmpw; i < tilesx; i++) {
			for (float j = r_oy / bmph; j < tilesy; j++) {
				glTexCoord2f(0.0f, 0.0f); glVertex2f(i * bmpw, j * bmph);
				glTexCoord2f(1.0f, 0.0f); glVertex2f((i + 1) * bmpw, j * bmph);
				glTexCoord2f(1.0f, 1.0f); glVertex2f((i + 1) * bmpw, (j + 1) * bmph);
				glTexCoord2f(0.0f, 1.0f); glVertex2f(i * bmpw, (j + 1) * bmph);
			}
			}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glUseProgram(0);
}
