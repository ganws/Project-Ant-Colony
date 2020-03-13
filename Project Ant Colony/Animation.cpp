#include "Animation.h"

Animation::Animation() {}
Animation::~Animation() {}

void Animation::InitAnimation(sf::Texture* texture, sf::Vector2u imageCount, int imageRealCount, float switchTime)
{
	this->m_imageCount = imageCount;
	this->m_imageRealCount = imageRealCount;
	this->m_switchTime = switchTime;
	m_totalTime = 0.0f;
	m_currentIndex = 0;
	m_currentImage.x = 0;
	m_uvRect.width = texture->getSize().x / float(imageCount.x);
	m_uvRect.height = texture->getSize().y / float(imageCount.y);
}

void Animation::Update(float deltaTime)
{
	m_totalTime += deltaTime;

	if (m_totalTime >= m_switchTime)
	{
		m_totalTime = 0.0f;
		m_currentImage.x += 7;
		m_currentIndex += 7;
		
		if (m_currentImage.x >= m_imageCount.x)
		{
			m_currentImage.x = 0;
			m_currentImage.y++;
		}
		
		if (m_currentIndex >= m_imageRealCount)
		{
			m_currentImage.y = 0;
			m_currentImage.x = 0;
			m_currentIndex = 0;
		}
			
	}

	m_uvRect.left = m_currentImage.x * m_uvRect.width;
	m_uvRect.top = m_currentImage.y * m_uvRect.height;
}