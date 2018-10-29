class Texture
{
public:
	Texture(const std::string&);
	~Texture();
	void Use();
	void setUniformInt(const std::string, unsigned int);
	unsigned int getID();
private:
	unsigned int TEX;

};
