#ifndef PIPELINE_H
#define PIPELINE_H
void pipeline_prepare(void);
void pipeline_modelToWorld(void);
void pipeline_cull(void);
void pipeline_worldToCamera(void);
void pipeline_cut(void);
void pipeline_perspect(void);
void pipeline_mapScreen(void);
void pipeline_render(void);
#endif
