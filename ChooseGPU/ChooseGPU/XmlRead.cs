using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace ChooseGPU
{
    class XmlRead
    {
        private XDocument xml;
        private XElement root;
        public XmlRead(string XmlFilePath, string RootTag)
        {
            xml = XDocument.Load(XmlFilePath);
            root = xml.Element(RootTag);
        }
        public static IEnumerable<XElement> GetElements(string GroupName, XElement Parent)
        {
            return Parent.Elements(GroupName);
        }
        public IEnumerable<XElement> GetElementsInRoot(string GroupName)
        {
            return root.Elements(GroupName);
        }
        public static string GetText(XElement Parent, string ElementName)
        {
            return Parent.Element(ElementName).Value;
        }
    }

}
