#include <render/RendererDeferredPhong.h>

GBuffer::GBuffer() {
	// Create the FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {
		m_textures[i].bindHidden();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, Darknec::WindowWidth, Darknec::WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i].glTexID_, 0);
	}

	// depth
	m_depthTexture.bindHidden();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, Darknec::WindowWidth, Darknec::WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
		NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture.glTexID_, 0);

	GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(4, DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void RendererDeferredPhong::render(Model model) {


	sys3->useShader("Text");


	Shader s = sys3->getShader("Text");
	//glBindTexture(GL_TEXTURE_2D, this->s);

	for (Mesh mesh : model.meshes_) {
		if (mesh.vao_ != NULL) {
			Material mat = model.materials_[mesh.materialIndex_];
			mat.getTex(Material::TEXTURE_DIFFUSE).bind(0);
			s.setUniformFloatVec4("frontMaterial.ambient", mat.getCol(Material::COLOUR_AMBIENT));
			s.setUniformFloatVec4("frontMaterial.diffuse", mat.getCol(Material::COLOUR_DIFFUSE));
			s.setUniformFloatVec4("frontMaterial.specular", mat.getCol(Material::COLOUR_SPECULAR));
			s.setUniformFloat("frontMaterial.shininess", 20.0f);

			glBindVertexArray(mesh.vao_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo_);
			glDrawElements(GL_TRIANGLES, mesh.numIndices_, GL_UNSIGNED_INT, 0);
			//glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}