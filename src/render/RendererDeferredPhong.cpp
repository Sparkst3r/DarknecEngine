#include <render/RendererDeferredPhong.h>


GBuffer::GBuffer() {
	glGenFramebuffers(1, &this->m_fbo);

	glBindFramebuffer(GL_FRAMEBUFFER, this->m_fbo);

	Texture texPosition(GL_TEXTURE_2D);
	texPosition.setDims(Darknec::WindowWidth, Darknec::WindowHeight);
	texPosition.setFormat(GL_RGBA, GL_RGBA);
	texPosition.setValueType(GL_UNSIGNED_BYTE);
	texPosition.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texPosition.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	texPosition.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texPosition.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texPosition.create();
	texPosition.bindToFrameBuffer(GL_COLOR_ATTACHMENT0);
	this->textures[0] = texPosition;

	Texture texNormal(GL_TEXTURE_2D);
	texNormal.setDims(Darknec::WindowWidth, Darknec::WindowHeight);
	texNormal.setFormat(GL_RGBA, GL_RGBA8_SNORM);
	texNormal.setValueType(GL_UNSIGNED_BYTE);
	texNormal.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texNormal.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	texNormal.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texNormal.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texNormal.create();
	texNormal.bindToFrameBuffer(GL_COLOR_ATTACHMENT1);
	this->textures[1] = texNormal;
	
	Texture texColour(GL_TEXTURE_2D);
	texColour.setDims(Darknec::WindowWidth, Darknec::WindowHeight);
	texColour.setFormat(GL_RGBA, GL_RGBA);
	texColour.setValueType(GL_UNSIGNED_BYTE);
	texColour.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texColour.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	texColour.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texColour.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texColour.create();
	texColour.bindToFrameBuffer(GL_COLOR_ATTACHMENT2);
	this->textures[2] = texColour;

	Texture texDepth(GL_TEXTURE_2D);
	texDepth.setDims(Darknec::WindowWidth, Darknec::WindowHeight);
	texDepth.setFormat(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32F);
	texDepth.setValueType(GL_UNSIGNED_BYTE);
	texDepth.setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
	texDepth.setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
	texDepth.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texDepth.setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texDepth.create();
	texDepth.bindToFrameBuffer(GL_DEPTH_ATTACHMENT);
	this->textures[3] = texDepth;


	GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, buffers);

	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE:
			return;
			break;
		default:
			printf("Unknown error while attempting to create frame buffer object!");
			break;
	}

}



void GBuffer::destroy() {
	glDeleteFramebuffers(1, &this->m_fbo);
	this->textures[0].destroy();
	this->textures[1].destroy();
	this->textures[2].destroy();
	this->textures[3].destroy();
	//this->textures[4].destroy();
}

void RendererDeferredPhong::init() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);



	float verts[] {
		-1.0, 1.0,
			1.0, 1.0,
			1.0, -1.0,
			-1.0, -1.0,
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	unsigned int Indices[] = {0, 1, 3, 2, 3, 1};
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

}

RendererDeferredPhong::RendererDeferredPhong() {
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
}

void RendererDeferredPhong::geometryPass(GBuffer* buf, Model model) {
	
	
	sys3->useShader("GeometryPass");
	

	glBindFramebuffer(GL_FRAMEBUFFER, buf->m_fbo);

	/*Shader s = sys3->getShader("GeometryPass");*/

	Shader s = sys3->getShader("GeometryPass");

	for (Mesh mesh : model.meshes_) {
		if (mesh.vao_ != NULL) {
			Material mat = model.materials_[mesh.materialIndex_];
			//s.setUniformFloatVec4("materialIn.ambient", mat.getCol(Material::COLOUR_AMBIENT));
			//s.setUniformFloatVec4("materialIn.diffuse", mat.getCol(Material::COLOUR_DIFFUSE));
			//s.setUniformFloatVec4("materialIn.specular", mat.getCol(Material::COLOUR_SPECULAR));
			//s.setUniformFloat("materialIn.shininess", 20.0f);
			mat.getTex(Material::TEXTURE_DIFFUSE).bind(0);
			glBindVertexArray(mesh.vao_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo_);
			glDrawElements(GL_TRIANGLES, mesh.numIndices_, GL_UNSIGNED_INT, 0);

		}
	}
}


void RendererDeferredPhong::frameStepBefore() {

	if (buffer != NULL) {
		buffer->destroy();
		delete buffer;
	}

	buffer = new GBuffer();

	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, Darknec::WindowWidth, Darknec::WindowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void RendererDeferredPhong::frameStepAfter() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	sys3->useShader("LightPass");

	buffer->textures[0].bind(0);
	buffer->textures[1].bind(1);
	buffer->textures[2].bind(2);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glActiveTexture(GL_TEXTURE0);

	sys3->useShader("GeometryPass");
	//glDeleteBuffers(1, &VBO);
	glEnable(GL_DEPTH_TEST);
}


void RendererDeferredPhong::render(Model model) {
	geometryPass(buffer, model);


	

}