/// 1. Construct MonoScript Component 
/// 2. Transform parent/children.
/// 3. Instantiate/Construct GameObject (Prefab)

using System;
using System.IO;
using System.Reflection;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml.Linq;
using System.Xml.Serialization;
using System.Runtime.CompilerServices;

namespace MonoCSharp
{
    public enum ObjectType
    {
        Object = 0,
        GameObject = 1,
        Component = 2,
        Transform = 3,
        MonoScript = 4,
        Scene = 999
    }

    public struct Vector3
    {
        public float x, y, z;
        public Vector3(float newx, float newy, float newz)
        {
            x = newx;
            y = newy;
            z = newz;
        }
        public override string ToString()
        {
            return (x + "," + y + "," + z);
        }
    }

    public class Scene
    {
        private IntPtr handle = (IntPtr)0;
        
        public GameObject[] gameObjects
        {
            get { return GetGameObjects(); }
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static GameObject[] GetGameObjects();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static Scene GetMainScene();
        
    }

    public class Object
    {
        protected IntPtr handle = (IntPtr)0;

        public IntPtr Handle
        {
            get { return this.handle; }
        }

        public static void Destory(Object obj)
        {
            DestroyViaPtr(obj.handle);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void DestroyViaPtr(IntPtr ptr);
    }

    public class GameObject : Object
    {
        public GameObject()
        {
            this.handle = GameObjectConstruct();
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr GameObjectConstruct();

        public string Name
        {
            get { return get_Name(this.Handle); }
            set { set_Name(this.Handle, value); }
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern string get_Name(IntPtr ptr);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void set_Name(IntPtr ptr, string str);

        public Component[] Components
        {
            get { return get_Components(this.Handle); }
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Component[] get_Components(IntPtr ptr);

        public int ComponentCount
        {
            get
            {
                return get_ComponentCount(this.handle);
            }
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int get_ComponentCount(IntPtr handle);

        public void AddComponent(Component component)
        {
            AddComponentViaPtr(this.Handle, component.Handle);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static int AddComponentViaPtr(IntPtr handleGameObject, IntPtr handleComponent);
    }
    
    public class Component: Object
    {
        public Component()
        {
            if(this.GetType() == typeof(Component))
            {
                this.handle = ComponentConstruct((int)ObjectType.Component);
            }
            else if (this.GetType() == typeof(Transform))
            {
                this.handle = ComponentConstruct((int)ObjectType.Transform);
            }
            else if (this is MonoScript)
            {
                string strScriptName = this.GetType().ToString();
                strScriptName = strScriptName.Substring(strScriptName.LastIndexOf('.') + 1);
                this.handle = ComponentConstruct((int)ObjectType.MonoScript, strScriptName);
            }
            else
            {
                Console.WriteLine("Unknown Component Type");
            }
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr ComponentConstruct(int tid, string name = null);


        public virtual int TID
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        }

    }

    public class Transform: Component
    {
        public override int TID
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        }

        public Vector3 Position
        {
            get { return get_Position(this.Handle); }
            set { set_Position(this.Handle, value); }
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Vector3 get_Position(IntPtr ptr);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void set_Position(IntPtr ptr, Vector3 pos);

        public Vector3 Rotation
        {
            get { return get_Rotation(this.Handle); }
            set { set_Rotation(this.Handle, value); }
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Vector3 get_Rotation(IntPtr ptr);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void set_Rotation(IntPtr ptr, Vector3 rot);

        public Vector3 Scale
        {
            get { return get_Scale(this.Handle); }
            set { set_Scale(this.Handle, value); }
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Vector3 get_Scale(IntPtr ptr);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void set_Scale(IntPtr ptr, Vector3 scale);
    }

    public class MonoScript: Component
    {
        //public string name;
    }

    public class MyMonoScript : MonoScript
    {

    }

    public class Core
    {
        public static void WorldUpdate()
        {
            //....
        }
    }

    public class TestClass
    {
        public static void HelloWorld()
        {
            Console.WriteLine("Hello CSharp!");
            Console.ReadLine();
        }
        public static void LogScene()
        {
            GameObject[] gameObjs = Scene.GetGameObjects();

            foreach (GameObject gameobject in gameObjs)
            {
                Console.WriteLine("==============GameObject: " + gameobject.Name + "===============");

                Component[] components = gameobject.Components;

                foreach (Component com in components)
                {
                    Console.WriteLine("----------------");
                    Console.WriteLine("TID：" + com.TID);

                    Type type = com.GetType();
                    Console.WriteLine(type.ToString());
                    if(type == typeof(Transform))
                    {
                        Transform t = (Transform)com;
                        Console.WriteLine("Value: (" + t.Position.x + "," + t.Position.y + "," + t.Position.z + ")");
                        Console.WriteLine("Value: (" + t.Rotation.x + "," + t.Rotation.y + "," + t.Rotation.z + ")");
                        Console.WriteLine("Value: (" + t.Scale.x + "," + t.Scale.y + "," + t.Scale.z + ")");
                    }
                    else
                    {
                        Console.WriteLine("NoValue");
                    }
                    Console.WriteLine("----------------");
                }

                Console.WriteLine("=======================================");
            }
        }
        public static void ComponentDeleteAddTest()
        {
            LogScene();

            Console.WriteLine("\n\n\n删除组件...\n\n\n");
            Console.WriteLine("\n\n\n重命名对象0...\n\n\n");
            Component com = Scene.GetGameObjects()[0].Components[0];//删除Transform
            Scene.GetGameObjects()[0].Name = "CSharp Renamed Object";
            Object.Destory(com);

            LogScene();

            Console.WriteLine("\n\n\n创建组件...");
            Transform newcom = new Transform();
            Console.WriteLine("添加组件...\n\n\n");
            newcom.Position = new Vector3(9f, 9f, 9f);
            newcom.Rotation = new Vector3(90f, 180f, 270f);
            newcom.Scale = new Vector3(1.1f, 1.1f, 1.1f);
            Scene.GetGameObjects()[0].AddComponent(newcom);//添加新的Transform

            LogScene();

            MyMonoScript t = new MyMonoScript();
            Scene.GetGameObjects()[0].AddComponent(t);

            LogScene();

            
            
            Console.ReadLine();
        }
    }

    
    
    public class Serializer
    {
        //Serialize //Save //ToPrefab
        //Deserialize //Instantiate

        public static void SaveScene(Scene scene)
        {
            List<XElement> gameObjectElesList = new List<XElement>();
            GameObject[] gameObjects = scene.gameObjects;
            foreach (var gameObject in gameObjects)
            {
                List<XElement> comElesList = new List<XElement>();
                foreach (var com in gameObject.Components)
                {
                    string strType = com.GetType().ToString();
                    strType = strType.Substring(strType.LastIndexOf('.') + 1);

                    List<XElement> PropertyElesList = new List<XElement>();
                    switch (strType)
                    {
                        case "Component":
                            PropertyElesList.Add(new XElement("NULL"));
                            break;
                        case "Transform":
                            Transform trans = com as Transform;
                            PropertyElesList.Add(new XElement("position", trans.Position.ToString()));
                            PropertyElesList.Add(new XElement("rotation", trans.Rotation.ToString()));
                            PropertyElesList.Add(new XElement("scale", trans.Scale.ToString()));
                            break;
                        default:
                            PropertyInfo[] propertyInfos = com.GetType().GetProperties();
                            FieldInfo[] fieldInfos = com.GetType().GetFields();

                            List<XElement> fieldAndPropertyElesList = new List<XElement>();
                            foreach (var propertyInfo in propertyInfos)
                            {
                                if(propertyInfo.Name != "Handle")
                                {
                                    XElement eleProperty = new XElement(propertyInfo.Name, propertyInfo.GetValue(com, null));
                                    fieldAndPropertyElesList.Add(eleProperty);
                                }
                            }

                            foreach (var fieldInfo in fieldInfos)
                            {
                                XElement eleField = new XElement(fieldInfo.Name, fieldInfo.GetValue(com));
                                fieldAndPropertyElesList.Add(eleField);
                            }


                            PropertyElesList.Add(new XElement("ScriptPropertiesAndFields", fieldAndPropertyElesList.ToArray()));
                            break;
                    }
                    

                    XElement eleCom = new XElement(strType, PropertyElesList.ToArray());
                    comElesList.Add(eleCom);
                }
                XElement eleGameObject = new XElement("GameObject", comElesList.ToArray());
                gameObjectElesList.Add(eleGameObject);
            }
            XDocument doc = new XDocument(
                new XDeclaration("1.0", "utf-8", "yes"),
                new XElement("Scene", gameObjectElesList.ToArray())
            );

            doc.Save(Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\Test.scene");
        }

        public static Scene LoadScene(string path) 
        {
            XDocument doc = XDocument.Load(path);
            XElement eleScene = doc.Element("Scene");

            foreach (XElement eleGameObject in eleScene.Elements())
            {
                //GameObject gameobj = new GameObject();
                //...
            }

            return null;
        }
    }
}
