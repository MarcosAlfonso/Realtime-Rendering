class BaseComponent
{
public:
	virtual ~BaseComponent() {};

	virtual void Init();
	virtual void Update();
	virtual void Destroy();

private:

};

