#include <render/RendererForwardPhong.h>

void RendererForwardPhong::render(Model model) {


	sys3->useShader("Text");


	Shader s = sys3->getShader("Text");
	//glBindTexture(GL_TEXTURE_2D, this->s);
	s.setUniformFloat4("frontMaterial.ambient", 0.1f, 0.0f, 0.0f, 1.0f);
	s.setUniformFloat4("frontMaterial.diffuse", 0.2f, 0.3f, 0.9f, 1.0f);
	s.setUniformFloat4("frontMaterial.specular", 0.2f, 0.3f, 0.9f, 1.0f);
	s.setUniformFloat("frontMaterial.shininess", 20.0f);

	for (Mesh mesh : model.meshes_) {
		if (mesh.vao_ != NULL) {
			glBindVertexArray(mesh.vao_);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo_);
			glDrawElements(GL_TRIANGLES, mesh.numIndices_, GL_UNSIGNED_INT, 0);
		}
	}
}