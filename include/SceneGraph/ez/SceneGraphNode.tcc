#include "ez/SceneGraphNode.h"

namespace ez
{
template <typename T>
SceneGraphNode<T>::SceneGraphNode(const T& inObject, const Transformation3f& inLocalTransformation)
{
  SetObject(inObject);
  SetLocalTransformation(inLocalTransformation);
}

template <typename T>
void SceneGraphNode<T>::SetLocalTransformation(const Transformation3f& inLocalTransformation)
{
  mLocalTransformation = inLocalTransformation;
}

template <typename T>
void SceneGraphNode<T>::SetParent(std::shared_ptr<SceneGraphNode> inSceneGraphNode)
{
  mParent = inSceneGraphNode;
}

template <typename T>
template <typename... TArgs>
std::shared_ptr<SceneGraphNode<T>> SceneGraphNode<T>::AddChild(TArgs&&... ioArgs)
{
  return AddChild(MakeSceneGraphNode<T>(std::forward<TArgs>(ioArgs)...));
}

template <typename T>
std::shared_ptr<SceneGraphNode<T>> SceneGraphNode<T>::AddChild(std::shared_ptr<SceneGraphNode> ioChild)
{
  EXPECTS(ioChild);
  mChildren.push_back(ioChild);
  ioChild->SetParent(std::enable_shared_from_this<SceneGraphNode<T>>::shared_from_this());
  return mChildren.back();
}

template <typename T>
Transformation3f SceneGraphNode<T>::GetTransformation() const
{
  if (const auto parent = mParent.lock())
    return parent->GetTransformation() * mLocalTransformation;
  return mLocalTransformation;
}
}