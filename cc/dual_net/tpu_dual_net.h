// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CC_DUAL_NET_TPU_DUAL_NET_H_
#define CC_DUAL_NET_TPU_DUAL_NET_H_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cc/constants.h"
#include "cc/model.h"
#include "cc/random.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/public/session.h"

namespace minigo {

class TpuDualNet : public Model {
 public:
  // tpu_name: The gRPC address of the VM's TPU, e.g. "grpc://10.240.2.10:8470".
  // graph_path: A frozen GraphDef proto suitable for running on TPU, e.g.
  //             generated by freeze_graph.py --use_tpu
  TpuDualNet(const std::string& tpu_name, const std::string& graph_path,
             const tensorflow::GraphDef& graph_def, int num_replicas);
  ~TpuDualNet() override;

 private:
  void RunManyImpl(std::string* model_name) override;
  void Reserve(size_t capacity);

  std::unique_ptr<tensorflow::Session> session_;
  std::vector<std::pair<std::string, tensorflow::Tensor>> inputs_;
  std::vector<std::string> output_names_;
  std::vector<tensorflow::Tensor> outputs_;
  size_t batch_capacity_ = 0;
  const int num_replicas_;
  const std::string graph_path_;
};

class TpuDualNetFactory : public ModelFactory {
 public:
  TpuDualNetFactory(std::string tpu_name);
  ~TpuDualNetFactory() override;

  std::unique_ptr<Model> NewModel(const std::string& descriptor) override;

 private:
  std::unique_ptr<tensorflow::Session> main_session_;
  std::string tpu_name_;
};

}  // namespace minigo

#endif  // CC_DUAL_NET_TPU_DUAL_NET_H_
