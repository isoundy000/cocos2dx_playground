#pragma once

#include <functional>

#include "2d/CCNode.h"

namespace graph_practice
{
	class GraphViewNode : public cocos2d::Node
	{
	public:
		using EvaluatorFunc = std::function<float( float )>;

	private:
		GraphViewNode( const EvaluatorFunc& evaluator_func );

	public:
		static GraphViewNode* create( const char* title, const int width, const int height, const EvaluatorFunc& evaluator_func );

	private:
		bool init( const char* title, const int width, const int height );

	public:
		//
		// g_x : zero to one
		//
		void UpdateView( const float g_x );

	private:
		cocos2d::Node* mViewNode;
		const EvaluatorFunc mEvaluatorFunc;
	};
}
