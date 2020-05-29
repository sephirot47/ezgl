#pragma once

#include "ez/Transformation.h"
#include <memory>
#include <vector>

namespace ez
{
template <typename T>
class SceneGraphNode : public std::enable_shared_from_this<SceneGraphNode<T>>
{
public:
  template <typename... TArgs>
  static std::shared_ptr<SceneGraphNode<T>> Make(TArgs&&... ioArgs);

  void SetLocalTransformation(const Transformation3f& inLocalTransformation);
  const Transformation3f& GetLocalTransformation() const { return mLocalTransformation; }
  Transformation3f GetTransformation() const;

  void SetParent(std::shared_ptr<SceneGraphNode> inSceneGraphNode);
  const std::weak_ptr<SceneGraphNode> GetParent() { return mParent; }
  const std::weak_ptr<const SceneGraphNode>& GetParent() const { return mParent; }

  template <typename... TArgs>
  std::shared_ptr<SceneGraphNode> AddChild(TArgs&&... ioArgs);
  std::shared_ptr<SceneGraphNode> AddChild(std::shared_ptr<SceneGraphNode> ioChild);
  const std::vector<std::shared_ptr<SceneGraphNode>>& GetChildren() const { return mChildren; }

  void SetObject(const T& inObject) { mObject = inObject; }
  T* GetObject() { return mObject ? &(mObject.value()) : nullptr; }
  const T* GetObject() const { return const_cast<SceneGraphNode&>(*this).GetObject(); }

  auto begin() { return mChildren.begin(); }
  auto end() { return mChildren.end(); }
  auto cbegin() const { return mChildren.cbegin(); }
  auto cend() const { return mChildren.cend(); }
  auto begin() const { return mChildren.cbegin(); }
  auto end() const { return mChildren.cend(); }

protected:
  SceneGraphNode() = default;
  SceneGraphNode(const T& inObject, const Transformation3f& inLocalTransformation = Identity<Transformation3f>());
  SceneGraphNode(const SceneGraphNode&) = delete;
  SceneGraphNode& operator=(const SceneGraphNode&) = delete;
  SceneGraphNode(SceneGraphNode&&) = default;
  SceneGraphNode& operator=(SceneGraphNode&&) = default;

private:
  std::weak_ptr<SceneGraphNode> mParent;
  std::vector<std::shared_ptr<SceneGraphNode>> mChildren;

  std::optional<T> mObject;
  Transformation3f mLocalTransformation = Identity<Transformation3f>();
};

template <typename T>
struct MakeSharedEnabler : public SceneGraphNode<T>
{
  template <typename... TArgs>
  MakeSharedEnabler(TArgs&&... ioPublicArgs) : SceneGraphNode<T>(std::forward<TArgs>(ioPublicArgs)...)
  {
  }
};

template <typename T>
template <typename... TArgs>
std::shared_ptr<SceneGraphNode<T>> SceneGraphNode<T>::Make(TArgs&&... ioArgs)
{
  return std::make_shared<MakeSharedEnabler<T>>(std::forward<TArgs>(ioArgs)...);
}

template <typename T, typename... TArgs>
auto MakeSceneGraphNode(TArgs&&... ioArgs)
{
  return SceneGraphNode<T>::Make(std::forward<TArgs>(ioArgs)...);
}

}

#include "ez/SceneGraphNode.tcc"