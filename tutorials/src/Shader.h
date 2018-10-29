#include <string>

class Shader
{
public:
	Shader(const std::string& shader_file);
	~Shader();
	void Use();
	void SetColor(float, float, float, float);
	void setUniformInt(const std::string uniString, unsigned int intInput);
	void setUniformMatrix4fv(const std::string uniString, float* intInput);
	unsigned int getID();

private:
	unsigned int prgm_id;
};
