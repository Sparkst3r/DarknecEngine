#include <render/RendererForwardPhong.h>

RendererForwardPhong::RendererForwardPhong() {
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RendererForwardPhong::init() {

}

void RendererForwardPhong::frameStepBefore() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0, 0, 0, 0);
	glViewport(0, 0, Darknec::WindowWidth, Darknec::WindowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
void RendererForwardPhong::frameStepAfter() {

}


void RendererForwardPhong::render(Model model) {
	
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